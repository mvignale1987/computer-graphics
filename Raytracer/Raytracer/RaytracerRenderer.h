#pragma once

#include <queue>
#include <SDL_mutex.h>
#include <SDL_atomic.h>
#include <SDL_timer.h>
#include "Renderer.h"
#include "Vector3.h"
#include "ImageBlock.h"
#include "BufferContent.h"
#include "Intersection.h"
#include "Ray.h"

class Material;

class RaytracerRenderer: public Renderer
{
public:
	// el tama�o de un bloque de imagen que agarra un thread
	static const unsigned int blockSize;
private:
	short					rayDepth;
	// cuanto se agranda la imagen; para hacer superSampling deber�a ser 2
	short					superSamplingConstant;

	// buffer
	unsigned int			bufferWidth;
	unsigned int			bufferHeight;
	BufferContent *			colorBuffer;
	BufferContent *			ambientBuffer;
	BufferContent *			diffuseBuffer;

	// synchronization vars
	SDL_mutex *				pendingBlocksMutex;
	std::queue<ImageBlock>	pendingBlocks;
	SDL_atomic_t			pendingThreads;

	// color buffer texture
	GLuint					colorTextureId;

	Uint32					raytracerStarted;
	Uint32					raytracerEnded;

	int						nThreads;
	bool					imageSaved;

	// camera orientation values
	double xmax;
	double ymax;
	Vector3 windowCenter;
	Vector3 rightWindowDirection;
	Vector3 upWindowDirection;

	// runtime rendering options
	bool linearTexture;
public:
	RaytracerRenderer(App &app, Scene &scene);

	// m�todos base del Renderer
	void init();
	void handleReshape(int h, int w);
	bool handleEvent(const SDL_Event &ev);
	void renderNextFrame();
private:
	// rendering
	void initOpenGL();
	void renderColorBuffer();
	static int saveImage(void *self);

	// inicio del rendering
	static int raytraceAsync(void *self);
	void prepareBlocks();
	static int processPendingBlocks(void *self);
	void processBlock(const ImageBlock& block);

	// raytracing
	Ray getRay(int x, int y);
	void rayTrace(const Ray& ray, int depth, Vector3& ambient, Vector3 &diffuse, Vector3& total);
	Intersection findFirstHit(const Ray& r);
	void shade(SceneObject *obj, const Ray& ray, const Vector3& intersectionPoint, int depth,
		Vector3& ambient, Vector3 &diffuse, Vector3& total);
	void shadePhong(SceneObject *obj, const Vector3& intersectionPoint, const Vector3& normal, Material& material,
		Vector3& ambient, Vector3 &diffuse, Vector3& total);
	void saveBufferToFile(BufferContent *buffer, const std::string& outPath);
};

