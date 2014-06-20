#pragma once

#include "Renderer.h"
#include "Vector3.h"
#include "RaytracerThread.h"
#include <SDL_opengl.h>

struct BufferContent 
{
	unsigned char r,g,b,a;

	BufferContent(unsigned char r = 0, unsigned char g = 0, unsigned char b = 0, unsigned char a = UCHAR_MAX);
	BufferContent(const Vector3& color, unsigned char a = UCHAR_MAX);
};

namespace RaytracerStatus {
	enum Status {
		INIT,			// generando imagen
		RENDER_DONE,	// imagen generada
		SAVED			// imagen guardada
	};
}

class RaytracerRenderer: public Renderer
{
public:
	// cuanto se agranda la imagen para hacer superSampling debería ser 2
	static const unsigned char superSamplingConstant;
private:
	size_t bufferSize;
	BufferContent *colorBuffer;
	bool colorBufferChanged;
	RaytracerStatus::Status status;
	GLuint colorTextureId;
	int nThreads;
public:
	RaytracerRenderer(App &app, Scene &scene);

	// métodos base del Renderer
	void init();
	void handleReshape(int h, int w);
	void renderNextFrame();
private:
	void renderColorBuffer();
	void saveImage();
};

