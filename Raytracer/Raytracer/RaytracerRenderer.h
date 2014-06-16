#pragma once

#include "Renderer.h"

class RaytracerRenderer: public Renderer
{
public:
	RaytracerRenderer(App &app, Scene &scene);

	void init();
	void handleReshape(int h, int w);
	void renderNextFrame();
};

