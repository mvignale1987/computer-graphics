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

const unsigned short RaytracerRenderer::superSamplingConstant = 2;
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
	bufferWidth(scene.imageWidth() * superSamplingConstant),
	bufferHeight(scene.imageHeight() * superSamplingConstant),
	colorBuffer(new BufferContent[bufferWidth * bufferHeight]),
	nThreads(SDL_GetCPUCount()),
	imageSaved(false),
	pendingBlocksMutex(SDL_CreateMutex())
{
	SDL_AtomicSet(&pendingThreads, nThreads);

	Camera camera = scene.camera();

	float midCameraFov = camera.fov() * 0.5f * (float) M_PI / 180.0f;

	xmax = sqrt(1.0/ cos(midCameraFov * midCameraFov) - 1);
	ymax = xmax * bufferHeight / bufferWidth;

	// TODO calcular las direcciones a partir de la orientación de la cámara
	windowCenter = camera.position() + Vector3::forward * camera.zNear();
	rightWindowDirection = Vector3::right;
	upWindowDirection = Vector3::up;
}

void RaytracerRenderer::init()
{
	Logger::log("Init OpenGL");
	initOpenGL();
	Logger::log("Init Buffer");
	unsigned int bufferSize = bufferWidth * bufferHeight * sizeof(BufferContent);
	memset(colorBuffer, 0xFF, bufferSize);
	
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
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
}

Ray RaytracerRenderer::getRay(int x, int y)
{
	Vector3 up    = (float)((2.0f * y / bufferHeight - 1) * ymax) * upWindowDirection;
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
	SDL_AtomicDecRef(&self.pendingThreads);

	return 0;
}

void RaytracerRenderer::processBlock(const ImageBlock& block)
{
	Logger::log("ProcessBlock " + block.toString());

	Vector3 bgColor = scene().backgroundColor();
	unsigned int maxX = block.width() + block.x();
	unsigned int maxY = block.height() + block.y();
	for(unsigned int y = block.y(); y < maxY; ++y)
	{
		for(unsigned int x = block.x(); x < maxX; ++x)
		{
			Ray r = getRay((int)x, (int)y);
			unsigned int position = y * bufferWidth + x;
			Vector3 color = rayTrace(r, 1);
			colorBuffer[position] = color;
		}
	}
}

Vector3 RaytracerRenderer::rayTrace(const Ray& ray, int depth)
{
	Intersection firstHit = findFirstHit(ray);
	if(firstHit.intersects())
	{
		Vector3 intersectionPoint = ray.origin() + firstHit.distance() * ray.direction();
		return shade(firstHit.obj(), intersectionPoint, depth);
	} else
		return scene().backgroundColor();
}

Intersection RaytracerRenderer::findFirstHit(const Ray& ray, SceneObject *caster)
{
	Intersection res = Intersection::noIntersection;
	vector<SceneObject *>& objects = scene().objects();
	for(vector<SceneObject *>::iterator it = objects.begin(); it != objects.end(); ++it)
	{
		if(*it == caster)
			continue;
		Intersection potential = (*it)->intersection(ray);
		if(potential.intersects() && (!res.intersects() || potential.distance() < res.distance()))
		{
			res = potential;
		}
	}
	return res;
}

Vector3 RaytracerRenderer::shade(SceneObject *obj, const Vector3& intersectionPoint, int depth)
{
	Vector3 normal = obj->normalAt(intersectionPoint).normalized();
	Vector3 eyeDirection = (scene().camera().position() - intersectionPoint).normalized();
	Material &material = *obj->material();
	Vector3 color = Vector3::zero;
	vector<Light>& lights = scene().lights();
	for(vector<Light>::iterator it = lights.begin(); it != lights.end(); ++it)
	{
		Vector3 lightVector = it->position() - intersectionPoint;
		Vector3 lightDirection = lightVector.normalized();
		Ray shadowRay(intersectionPoint, lightDirection);

		Vector3 lightColor = it->ambientColor().multiply(material.ambientColor()) * material.ambientCoefficient(); 

		Intersection intersection = findFirstHit(shadowRay, obj);
		
		if(!intersection)
		{
			Vector3 reflectedRay = lightDirection - 2 * normal * (lightDirection * normal);

			float invAttenuation = (it->linearAttenuation() * lightVector.length() + it->quadAttenuation() * lightVector.lengthSquared() );
			float attenuationFactor = 1 / invAttenuation;

			Vector3 diffuseColor = material.diffuseCoefficient() * material.diffuseColor() * (normal * lightDirection);
			Vector3 specularColor = material.specularCoefficient() * material.specularColor() * powf(reflectedRay * eyeDirection, material.specularExponent());
			Vector3 diffuseLightColor = attenuationFactor * it->diffuseColor().multiply(diffuseColor + specularColor);
			lightColor += diffuseLightColor;
		}
		
		color += lightColor; 
	}
	return color.clamped();
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
		Logger::log("Saving image");
		saveImage();
		imageSaved = true;
		Logger::log("Saved image");
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

void RaytracerRenderer::saveImage()
{
	static const int maxTimeLength = 255;
	char timeStr[maxTimeLength];
	time_t rawtime;
	time (&rawtime);
	tm timeinfo;
	localtime_s(&timeinfo, &rawtime);
	strftime (timeStr, maxTimeLength, "%Y-%m-%d %H.%M.%S", &timeinfo);
	string outPath = scene().outputDir() + '\\' + timeStr + ".png";

	CreateDirectory(scene().outputDir().c_str(), NULL);

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
			BufferContent& color = colorBuffer[position];

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


