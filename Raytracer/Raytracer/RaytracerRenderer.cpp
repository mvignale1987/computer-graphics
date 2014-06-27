#include "RaytracerRenderer.h"
#include <sstream>
#include <ctime>
#include <deque>
#include <set>
#include <SDL_opengl.h>
#include <SDL_thread.h>
#include <FreeImage.h>
#include <GL/GLU.h>
#include "Scene.h"
#include "App.h"
#include "SceneError.h"
#include "Logger.h"

using namespace std;

const unsigned int RaytracerRenderer::blockSize = 256;

static void lockMutex(SDL_mutex *mutex)
{
	if (SDL_LockMutex(mutex) != 0)
		throw SceneError::fromSDLError("Couldn't lock mutex");
}

static void unlockMutex(SDL_mutex *mutex)
{
	if (SDL_UnlockMutex(mutex) != 0)
		throw SceneError::fromSDLError("Couldn't unlock mutex");
}

RaytracerRenderer::RaytracerRenderer(App &app, Scene &scene):
	Renderer(app, scene),
	rayDepth(scene.rayDepth()),
	superSamplingConstant(scene.supersampling()),
	bufferWidth(scene.imageWidth() * scene.supersampling()),
	bufferHeight(scene.imageHeight() * scene.supersampling()),
	colorBuffer(new BufferContent[bufferWidth * bufferHeight]),
	ambientBuffer(new BufferContent[bufferWidth * bufferHeight]),
	diffuseBuffer(new BufferContent[bufferWidth * bufferHeight]),
	nThreads(SDL_GetCPUCount()),
	imageSaved(false),
	pendingBlocksMutex(SDL_CreateMutex()),
	linearTexture(true),
	raytracerStarted(0)
{
	SDL_AtomicSet(&pendingThreads, nThreads);

	Camera camera = scene.camera();

	float midCameraFov = camera.fov() * 0.5f * (float) M_PI / 180.0f;

	xmax = sqrt(1.0/ cos(midCameraFov * midCameraFov) - 1);
	ymax = xmax * ((float)bufferHeight / (float)bufferWidth) ;

	// TODO calcular las direcciones a partir de la orientación de la cámara
	windowCenter = camera.position() + Vector3::forward * camera.zNear();
	rightWindowDirection = Vector3::right;
	upWindowDirection = Vector3::up;
}

void RaytracerRenderer::init()
{
	initOpenGL();
	unsigned int bufferSize = bufferWidth * bufferHeight * sizeof(BufferContent);
	memset(colorBuffer, 0xFF, bufferSize);
	memset(ambientBuffer, 0x00, bufferSize);
	memset(diffuseBuffer, 0x00, bufferSize);

	raytracerStarted = SDL_GetTicks();

	Logger::log("Init Raytracing");

	SDL_Thread *thread = SDL_CreateThread(raytraceAsync, "RT_Init", this);
	SDL_DetachThread(thread); 
}

void RaytracerRenderer::initOpenGL()
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glClearColor(Vector3::zero);
	glEnable(GL_TEXTURE_2D);
	glShadeModel(GL_SMOOTH);		 // Enables Smooth Shading
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculation

	glGenTextures(1, &colorTextureId);
	// Use this texture id
	glBindTexture(GL_TEXTURE_2D, colorTextureId);
	// Associate this texture id with some texture-specific data
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, linearTexture ? GL_LINEAR : GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,  linearTexture ? GL_LINEAR : GL_NEAREST);
}

bool RaytracerRenderer::handleEvent(const SDL_Event &ev)
{
	if(ev.type == SDL_KEYDOWN && ev.key.keysym.sym == SDLK_l)
	{
		linearTexture = !linearTexture;
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, linearTexture ? GL_LINEAR : GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,  linearTexture ? GL_LINEAR : GL_NEAREST);
	}

	return true;
}

Ray RaytracerRenderer::getRay(int x, int y)
{
	Vector3 up    = (float)((1 - 2.0f * y / bufferHeight) * ymax) * upWindowDirection;
	Vector3 right = (float)((2.0f * x / bufferWidth  - 1) * xmax) * rightWindowDirection;
	Vector3 referencePoint = up + right + windowCenter;
	Vector3 origin = scene().camera().position();
	return Ray(origin, referencePoint - origin);
}

void RaytracerRenderer::prepareBlocks()
{
	typedef pair<unsigned int, unsigned int> pairUint;

	// initialize image to sample output color
	unsigned int blockHorizontalSize = (unsigned int) ceil((double)bufferWidth / blockSize);
	unsigned int blockVerticalSize = (unsigned int) ceil((double)bufferHeight / blockSize);

	set<pairUint> visited;
	deque<pairUint> pendingPut;

	pendingPut.push_front(pairUint(blockHorizontalSize/2, blockVerticalSize/2));

	while(!pendingPut.empty())
	{
		pairUint firstPendingPut = pendingPut.front();
		pendingPut.pop_front();

		if(firstPendingPut.first >= blockHorizontalSize)
			continue;
		if(firstPendingPut.second >= blockVerticalSize)
			continue;
		if(visited.find(firstPendingPut) != visited.end())
			continue;

		visited.insert(firstPendingPut);

		unsigned int x = blockSize * firstPendingPut.first;
		unsigned int y = blockSize * firstPendingPut.second;
		unsigned int sizeX = min<unsigned int>(x + blockSize, bufferWidth) - x;
		unsigned int sizeY = min<unsigned int>(y + blockSize, bufferHeight) - y;
		pendingBlocks.push(ImageBlock(x,y,sizeX, sizeY));

		pairUint left = pairUint(firstPendingPut.first-1, firstPendingPut.second);
		pendingPut.push_back(left);

		pairUint right = pairUint(firstPendingPut.first+1, firstPendingPut.second);
		pendingPut.push_back(right);

		pairUint up = pairUint(firstPendingPut.first, firstPendingPut.second-1);
		pendingPut.push_back(up);

		pairUint down = pairUint(firstPendingPut.first, firstPendingPut.second+1);
		pendingPut.push_back(down);
	}
}

int RaytracerRenderer::raytraceAsync(void *s)
{
	RaytracerRenderer &self = *(RaytracerRenderer *) s;
	self.prepareBlocks();

	for(int i = 0; i < self.nThreads; ++i)
	{
		stringstream ss;
		ss << "RT_ProcessPendingBlocks_" << i+1;
		SDL_Thread *thread = SDL_CreateThread(processPendingBlocks, ss.str().c_str(), &self);
		SDL_DetachThread(thread); 
	}

	return 0;
}

int RaytracerRenderer::processPendingBlocks(void *s)
{
	srand(SDL_GetTicks()); // para colores random (debug)

	RaytracerRenderer &self = *(RaytracerRenderer *) s;
	for(bool emptyQueue = false; !emptyQueue; )
	{		
		ImageBlock block;
		lockMutex(self.pendingBlocksMutex);
		{
			emptyQueue = self.pendingBlocks.empty();
			if(!emptyQueue)
			{
				block = self.pendingBlocks.front();
				self.pendingBlocks.pop();
			}
		}
		unlockMutex(self.pendingBlocksMutex);

		if(!emptyQueue)
		{
			self.processBlock(block);
		}
	}

	int pendingBlocksBefore = SDL_AtomicAdd(&self.pendingThreads, -1);
	if(pendingBlocksBefore == 1)
	{
		self.raytracerEnded = SDL_GetTicks();
	}

	return 0;
}

void RaytracerRenderer::processBlock(const ImageBlock& block)
{
	Vector3 bgColor = scene().backgroundColor();
	unsigned int maxX = block.width() + block.x();
	unsigned int maxY = block.height() + block.y();
	for(unsigned int y = block.y(); y < maxY; ++y)
	{
		for(unsigned int x = block.x(); x < maxX; ++x)
		{
			Ray r = getRay((int)x, (int)y);
			unsigned int position = y * bufferWidth + x;
			Vector3  ambient, diffuse, total;
			rayTrace(r, rayDepth, ambient, diffuse, total);
			colorBuffer[position] = total.clamped();
			ambientBuffer[position] = ambient.clamped();
			diffuseBuffer[position] = diffuse.clamped();
		}
	}
}

void RaytracerRenderer::rayTrace(const Ray& ray, int depth, Vector3& ambient, Vector3 &diffuse, Vector3& total)
{
	Intersection firstHit = findFirstHit(ray);
	if(firstHit.intersects())
	{
		Vector3 intersectionPoint = ray.origin() + firstHit.distance() * ray.direction();
		shade(firstHit.obj(), ray, intersectionPoint, depth, ambient, diffuse, total);
	} else {
		total = scene().backgroundColor();
		ambient = diffuse = Vector3::zero;
	}
}

Intersection RaytracerRenderer::findFirstHit(const Ray& ray)
{
	Intersection res = Intersection::noIntersection;
	vector<SceneObject *>& objects = scene().objects();
	for(vector<SceneObject *>::iterator it = objects.begin(); it != objects.end(); ++it)
	{
		if(!(*it)->aabb().intersects(ray))
			continue;

		Intersection potential = (*it)->intersection(ray);
		if(potential.intersects() && (!res.intersects() || potential.distance() < res.distance()))
		{
			res = potential;
		}
	}
	return res;
}

void RaytracerRenderer::shadePhong(SceneObject *obj, const Vector3& intersectionPoint, const Vector3& normal, Material& material,
		Vector3& ambient, Vector3 &diffuse, Vector3& color)
{
	
	Vector3 eyeDirection = (scene().camera().position() - intersectionPoint).normalized();
	vector<Light>& lights = scene().lights();

	Vector3 materialDiffuseColor;
	Vector3 materialAmbientColor;

	if(material.textured())
	{
		Vector2 uv = obj->textureCoordinatesAt(intersectionPoint);
		Vector3 texturedColor = material.getColorAt(uv);
		materialDiffuseColor = materialAmbientColor = texturedColor;
	} else 
	{
		materialAmbientColor = material.ambientColor();
		materialDiffuseColor = material.diffuseColor();
	}

	for(vector<Light>::iterator it = lights.begin(); it != lights.end(); ++it)
	{
		Vector3 lightVector = it->position() - intersectionPoint;
		Vector3 lightDirection = lightVector.normalized();

		Vector3 ambientColor = it->ambientColor().multiply(materialAmbientColor) * material.ambientCoefficient(); 
		ambient += ambientColor;
		color += ambientColor; 

		Intersection shadowIntersection = findFirstHit(Ray(intersectionPoint, lightDirection));

		if(shadowIntersection.intersects() && shadowIntersection.distance() < (intersectionPoint - it->position()).length())
		{
			continue;
		}

		Vector3 reflectedRay = 2 * normal * (normal * lightDirection) - lightDirection;

		float invAttenuation = (it->linearAttenuation() * lightVector.length() + it->quadAttenuation() * lightVector.lengthSquared() );
		if(invAttenuation <= 0)
			invAttenuation = 1;
		float attenuationFactor = min<float>(1 / invAttenuation, 1);

		Vector3 diffuseColor;
		if(material.diffuseCoefficient() > 0)
		{
			diffuseColor = material.diffuseCoefficient() * materialDiffuseColor * max<float>(normal * lightDirection, 0);
			diffuse += diffuseColor;
		}

		Vector3 specularColor;
		if(material.specularCoefficient() > 0)
		{
			float specularExponent = powf(max<float>(reflectedRay * eyeDirection, 0), material.specularExponent());
			specularColor = material.specularCoefficient() * material.specularColor() * specularExponent;
		}

		Vector3 nonambientColor = attenuationFactor * it->diffuseColor().multiply(specularColor + diffuseColor);
		if(nonambientColor.x() < 0 || nonambientColor.y() < 0 || nonambientColor.z() < 0)
		{
			nonambientColor = nonambientColor.clamped();
		}
		color += nonambientColor;
	}
}

void RaytracerRenderer::shade(SceneObject *obj, const Ray& ray, const Vector3& intersectionPoint, int depth, 
							  Vector3& ambient, Vector3 &diffuse, Vector3& color)
{
	color = ambient = diffuse = Vector3::zero;

	Material &material = *obj->material();
	Vector3 normal = obj->normalAt(ray, intersectionPoint).normalized();
	if(material.hasNormalMap())
	{
		Vector2 uv = obj->textureCoordinatesAt(intersectionPoint);
		Vector3 rawNormal = material.getNormalAt(uv);
		normal = (
			rawNormal.x() * obj->xTextureVector(intersectionPoint) + 
			rawNormal.y() * obj->yTextureVector(intersectionPoint) +
			rawNormal.z() * normal
			).normalized();
	}

	shadePhong(obj, intersectionPoint, normal, material, ambient, diffuse, color);

	if(depth == 0) 
	{
		return;
	}

	if(material.mirrored())
	{
		Vector3 colorMirrored, diffuseMirrored, ambientMirrored;
		rayTrace(Ray(intersectionPoint, normal), depth-1, ambientMirrored, diffuseMirrored, colorMirrored);
		color += material.specularCoefficient() * colorMirrored;
	} 
	if(material.refractive())
	{
		Vector3 colorRefraction, diffuseRefraction, ambientRefraction;

		Vector3 I = ray.direction().normalized();
		float eta, c1, cs2 ;
		c1 = - I * normal;
		eta = c1 > 0 ? 1 / material.refractionIndex(): material.refractionIndex();			
		cs2 = 1 - eta * eta * (1 - c1 * c1) ;

		if (cs2 < 0)
			return ;		/* total internal reflection */
	
		Vector3 rayDirection = eta * I + (eta * c1 - sqrt(cs2)) * normal;
		rayTrace(Ray(intersectionPoint, rayDirection), depth-1, ambientRefraction, diffuseRefraction, colorRefraction);
		color += material.transparency() * colorRefraction;
	}
}

void RaytracerRenderer::handleReshape(int width, int height)
{
	if (height==0)
	{
		height=1;
	}
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-width, width, -height, height);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	renderColorBuffer();
}

void RaytracerRenderer::renderNextFrame()
{
	renderColorBuffer();

	int pendingThreadsVal = SDL_AtomicGet(&pendingThreads);
	if(pendingThreadsVal == 0 && !imageSaved)
	{
		stringstream ss;
		ss << "Finished Rendering in " << (raytracerEnded - raytracerStarted) / 1000.0f << "s. Saving image...";
		Logger::log(ss.str());
		SDL_Thread *thread = SDL_CreateThread(saveImage, "RT_SaveImage", this);
		SDL_DetachThread(thread); 
		imageSaved = true;
	}
}

void RaytracerRenderer::renderColorBuffer()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	int w = (int) bufferWidth;
	int h = (int) bufferHeight;
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, colorBuffer);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	// One vertex is (2 + 2)*4 bytes (2 GLfloats for position, 2 GLfloats for texture coordinates, 4 bytes per GLfloat)
	const int SIZE_OF_VERTEX = (2 + 2)*sizeof(GLfloat);

	GLfloat data[] = {// 2 texture coordinates followed by 2 vertex coordinates in counterclockwise order
		0, 0, -1, +1,// Bottomleft
		1, 0, +1, +1,// Bottomright
		1, 1, +1, -1,// Topright
		0, 1, -1, -1,// Topleft
	};

	// Use this texture
	glBindTexture(GL_TEXTURE_2D, colorTextureId);

	// Tell OpenGL where our vertex data begins and how big each vertex is
	glTexCoordPointer(2, GL_FLOAT, SIZE_OF_VERTEX, data+0);// Texture data begins at index 0
	glVertexPointer  (2, GL_FLOAT, SIZE_OF_VERTEX, data+2);// Vertex  data begins at index 2

	int winWidth, winHeight;
	SDL_GetWindowSize(app().getWindow(), &winWidth, &winHeight);

	GLfloat imageAspectRatio = (GLfloat) scene().imageWidth() / (GLfloat) scene().imageHeight();
	GLfloat windowAspectRatio = (GLfloat) winWidth / (GLfloat) winHeight;

	// makes image fit in window
	if(imageAspectRatio > windowAspectRatio)
	{
		glScalef((GLfloat)winWidth, winHeight * windowAspectRatio / imageAspectRatio, 1.0f);
	} else {
		glScalef(winWidth * imageAspectRatio / windowAspectRatio, (GLfloat)winHeight, 1.0f);
	}

	// Draw data
	glDrawArrays(GL_QUADS, 0, 4);
}

static string getTimeStr()
{
	static const int maxTimeLength = 255;
	char timeStr[maxTimeLength];
	time_t rawtime;
	time (&rawtime);
	tm timeinfo;
	localtime_s(&timeinfo, &rawtime);
	strftime (timeStr, maxTimeLength, "%Y-%m-%d %H.%M.%S", &timeinfo);

	return string(timeStr);
}

int RaytracerRenderer::saveImage(void *s)
{
	RaytracerRenderer *self = (RaytracerRenderer *)s;

	string outputDir =  self->scene().outputDir() ;

	CreateDirectory(outputDir.c_str(), NULL);

	string timeStr = getTimeStr();
	string outPathColor   = outputDir + '\\' + timeStr + "-total.png";
	string outPathAmbient = outputDir + '\\' + timeStr + "-ambient.png";
	string outPathDiffuse = outputDir + '\\' + timeStr + "-diffuse.png";

	self->saveBufferToFile(self->colorBuffer, outPathColor);
	self->saveBufferToFile(self->ambientBuffer, outPathAmbient);
	self->saveBufferToFile(self->diffuseBuffer, outPathDiffuse);

	Logger::log("Saved image at '" + outPathColor + "' (ambient & diffuse saved too)");

	return 0;
} 

void RaytracerRenderer::saveBufferToFile(BufferContent *buffer, const std::string& outPath)
{
	FIBITMAP *dib = FreeImage_Allocate(bufferWidth, bufferHeight, 32,
		FI_RGBA_RED_MASK, FI_RGBA_GREEN_MASK, FI_RGBA_BLUE_MASK);
	if(!dib)
		throw SceneError("saveImage: couldn't allocate image");

	// Calculate the number of bytes per pixel (3 for 24-bit or 4 for 32-bit)
	int bytespp = FreeImage_GetLine(dib) / FreeImage_GetWidth(dib);
	for(unsigned y = 0; y < FreeImage_GetHeight(dib); y++) {
		BYTE *bits = FreeImage_GetScanLine(dib, y);
		for(unsigned x = 0; x < FreeImage_GetWidth(dib); x++) {
			size_t position = (bufferHeight - y - 1) * bufferWidth + x;
			BufferContent& color = buffer[position];

			bits[FI_RGBA_RED] = color.r;
			bits[FI_RGBA_GREEN] = color.g;
			bits[FI_RGBA_BLUE] = color.b;
			bits[FI_RGBA_ALPHA] = color.a;

			bits += bytespp;
		}
	}

	FIBITMAP *scaledDib = FreeImage_Rescale(dib, scene().imageWidth(), scene().imageHeight());

	if(!FreeImage_Save(FIF_PNG, scaledDib, outPath.c_str(), PNG_Z_BEST_COMPRESSION))
		throw SceneError("saveImage: save image");
}
