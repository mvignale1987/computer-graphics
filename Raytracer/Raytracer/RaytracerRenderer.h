#pragma once

#include "Renderer.h"
#include "Vector3.h"
#include <SDL_opengl.h>

struct BufferContent 
{
	unsigned char r,g,b,a;

	BufferContent(unsigned char r = 0, unsigned char g = 0, unsigned char b = 0, unsigned char a = UCHAR_MAX);
	BufferContent(const Vector3& color, unsigned char a = UCHAR_MAX);
};

class RaytracerRenderer: public Renderer
{
public:
	// cuanto se agranda la imagen para hacer superSampling debería ser 2
	static const unsigned char superSamplingConstant;
private:
	size_t bufferSize;
	BufferContent *colorBuffer;
	bool colorBufferChanged;
	GLuint colorTextureId;
public:
	RaytracerRenderer(App &app, Scene &scene);

	void init();
	void handleReshape(int h, int w);
	void renderNextFrame();
private:
	void renderColorBuffer();
};

