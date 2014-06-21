#include "RaytracerRenderer.h"
#include <sstream>
#include <ctime>
#include <SDL_opengl.h>
#include <SDL_thread.h>
#include <GL/GLU.h>
#include <IL/devil_cpp_wrapper.hpp>
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

	xmax = sqrt(1.0/ cos(camera.fov() * 0.5) - 1);
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
	size_t bufferSize = bufferWidth * bufferHeight * sizeof(BufferContent);
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
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
}

Ray RaytracerRenderer::getRay(int x, int y)
{
	Vector3 up    = (float)((2.0f * y / bufferHeight - 1) * ymax) * upWindowDirection;
	Vector3 right = (float)((2.0f * x / bufferWidth  - 1) * xmax) * rightWindowDirection;
	Vector3 referencePoint = up + right + windowCenter;
	return Ray(scene().camera().position(), referencePoint);
}

void RaytracerRenderer::prepareBlocks()
{
	// initialize image to sample output color
	size_t blockHorizontalSize = (size_t) ceil((double)bufferWidth / blockSize);
	size_t blockVerticalSize = (size_t) ceil((double)bufferHeight / blockSize);

	for(size_t i = 0; i < blockVerticalSize; ++i){
		for(size_t j = 0; j < blockHorizontalSize; ++j){
			size_t x = blockSize * j;
			size_t y = blockSize * i;
			size_t sizeX = min<size_t>(x + blockSize, bufferWidth) - x;
			size_t sizeY = min<size_t>(y + blockSize, bufferHeight) - y;
			pendingBlocks.push(ImageBlock(x,y,sizeX, sizeY));
		}
	}
}

int RaytracerRenderer::raytraceAsync(void *s)
{
	RaytracerRenderer &self = *(RaytracerRenderer *) s;

	Logger::log("PrepareBlocks");
	self.prepareBlocks();

	for(int i = 0; i < self.nThreads; ++i)
	{
		stringstream ss;
		ss << "RT_ProcessPendingBlocks_" << i+1;
		SDL_Thread *thread = SDL_CreateThread(processPendingBlocks, ss.str().c_str(), &self);
		SDL_DetachThread(thread); 
	}
	Logger::log("Done init");

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

	Vector3 bgColor = Vector3::random(); //scene().backgroundColor();
	size_t maxX = block.width() + block.x();
	size_t maxY = block.height() + block.y();
	for(size_t y = block.y(); y < maxY; ++y)
	{
		for(size_t x = block.x(); x < maxX; ++x)
		{
			Ray r = getRay(x, y);
			size_t position = y * bufferWidth + x;
			colorBuffer[position] = BufferContent(bgColor);
		}
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

	ILuint imageID = ilGenImage();
	ilBindImage(imageID);
	ilTexImage(
		(ILuint) bufferWidth,
		(ILuint) bufferHeight,
		1,  // OpenIL supports 3d textures!  but we don't want it to be 3d. so we just set this to be 1
		4,  //  channels: one for R , one for G, one for B, one for A
		IL_RGBA,  // duh, yeah use rgba
		IL_UNSIGNED_BYTE,  // the type of data the imData array contains (next)
		colorBuffer  // and the array of bytes represneting the actual image data
	);
	ilEnable(IL_FILE_OVERWRITE);
	iluScale(scene().imageWidth(), scene().imageHeight(), 1);
	ilSave( IL_PNG, outPath.c_str());

	ILenum ilError = ilGetError();
	if(ilError != IL_NO_ERROR)
	{
		throw SceneError::fromILError(ilError, "Error while saving image");
	}
} 


