#pragma once
#include <SDL.h>

class App;
class Scene;

class Renderer
{
private:
	App &parentApp;
	Scene &childScene;
protected:
	Renderer(App &app, Scene &scene);
public:
	App& app();
	Scene &scene();
	virtual void init();
	virtual void handleReshape(int h, int w);
	virtual bool handleEvent(const SDL_Event &ev);
	virtual void renderNextFrame() = 0;
	virtual void clean();

	virtual ~Renderer();
};