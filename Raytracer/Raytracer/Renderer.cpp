#include "Renderer.h"
#include "App.h"

Renderer::Renderer(App &app, Scene &scene):
	parentApp(app),
	childScene(scene)
{
}

App& Renderer::app()
{
	return parentApp;
}

void Renderer::init()
{
}

void Renderer::handleReshape(int h, int w)
{
}

bool Renderer::handleEvent(const SDL_Event &ev)
{
	return true;
}

 void Renderer::clean()
{
}

 Renderer::~Renderer()
 {
 }