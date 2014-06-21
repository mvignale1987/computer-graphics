#pragma once

#include <queue>
#include <SDL_mutex.h>
#include <SDL_atomic.h>
#include "Renderer.h"
#include "Vector3.h"
#include "ImageBlock.h"
#include "BufferContent.h"
#include "Ray.h"

class RaytracerRenderer: public Renderer
{
public:
	// cuanto se agranda la imagen para hacer superSampling deber�a ser 2
	static const unsigned short superSamplingConstant;
	// el tama�o de un bloque de imagen que agarra un thread
	static const unsigned int blockSize;
private:
	// buffer
	size_t					bufferWidth;
	size_t					bufferHeight;
	BufferContent *			colorBuffer;

	// synchronization vars
	SDL_mutex *				pendingBlocksMutex;
	std::queue<ImageBlock>	pendingBlocks;
	SDL_atomic_t			pendingThreads;

	// color buffer texture
	GLuint					colorTextureId;

	int						nThreads;
	bool					imageSaved;

	// camera orientation values
	double xmax;
	double ymax;
	Vector3 windowCenter;
	Vector3 rightWindowDirection;
	Vector3 upWindowDirection;
public:
	RaytracerRenderer(App &app, Scene &scene);

	// m�todos base del Renderer
	void init();
	void handleReshape(int h, int w);
	void renderNextFrame();
private:
	void renderColorBuffer();
	void saveImage();
	static int raytraceAsync(void *self);
	static int processPendingBlocks(void *self);
	void processBlock(const ImageBlock& block);
	Ray  getRay(int x, int y);
	void prepareBlocks();
	void initOpenGL();
};

