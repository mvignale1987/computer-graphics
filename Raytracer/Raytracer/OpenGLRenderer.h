#pragma once

#include "Renderer.h"

class Camera;
class SceneObjectSphere;
class SceneObjectQuad;
class SceneObjectTriangle;
class SceneObjectCyllinder;

class OpenGLRenderer: public Renderer
{
public:
	OpenGLRenderer(App &app, Scene &scene);

	void init();
	void handleReshape(int w, int h);
	void renderNextFrame();
private:
	Camera getCorrectedCamera();
	void renderSphere(SceneObjectSphere *sphere);
	void renderCyllinder(SceneObjectCyllinder *cyllinder);
	void renderQuad(SceneObjectQuad *quad);
	void renderTriangle(SceneObjectTriangle *triangle);
};