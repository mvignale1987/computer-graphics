#pragma once

#include "Renderer.h"

class OpenGLRenderer: public Renderer
{
public:
	OpenGLRenderer(App &app, Scene &scene);

	void handleReshape(int h, int w);
	void renderNextFrame();
};