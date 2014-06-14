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

Scene& Renderer::scene()
{
	return childScene;
}

void Renderer::init()
{
}

void Renderer::handleReshape(int , int )
{
}

bool Renderer::handleEvent(const SDL_Event &)
{
	return true;
}

 void Renderer::clean()
{
}

 Renderer::~Renderer()
 {
 }