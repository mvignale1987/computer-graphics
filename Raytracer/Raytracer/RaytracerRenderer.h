#pragma once

#include <queue>
#include <SDL_mutex.h>
#include <SDL_atomic.h>
#include "Renderer.h"
#include "Vector3.h"
#include "ImageBlock.h"
#include "BufferContent.h"
#include "Intersection.h"
#include "Ray.h"

class RaytracerRenderer: public Renderer
{
public:
	// cuanto se agranda la imagen para hacer superSampling debería ser 2
	static const unsigned short superSamplingConstant;
	// el tamaño de un bloque de imagen que agarra un thread
	static const unsigned int blockSize;
private:
	// buffer
	unsigned int			bufferWidth;
	unsigned int			bufferHeight;
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

	// métodos base del Renderer
	void init();
	void handleReshape(int h, int w);
	void renderNextFrame();
private:
	// rendering
	void initOpenGL();
	void renderColorBuffer();
	void saveImage();

	// inicio del rendering
	static int raytraceAsync(void *self);
	void prepareBlocks();
	static int processPendingBlocks(void *self);
	void processBlock(const ImageBlock& block);

	// raytracing
	Ray getRay(int x, int y);
	Vector3 rayTrace(const Ray& ray, int depth);
	Intersection findFirstHit(const Ray& r, SceneObject *caster = NULL);
	Vector3 shade(SceneObject *obj, const Vector3& intersectionPoint, int depth);
};

