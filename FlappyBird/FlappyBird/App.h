#pragma once

#include <SDL.h>
#include <vector>
#include <string>

class Scene;

class App
{
private:
	Scene *currentScene;
	SDL_Window *win;
	SDL_Renderer *ren;
	SDL_GLContext glContext;
	float frameTime;
	unsigned long nFrames;

	// constants
	static const std::string windowTitle;
	static const std::string appIconPath;
public:
	App();
	int run(int argc, char **argv);

	/* scene management */
	void setScene(Scene *scene);
	Scene *getScene();

	/* getters */
	SDL_Window *getWindow() const;
	SDL_Renderer *getRenderer() const;
	SDL_GLContext getGlContext() const;
	float getFrameTime() const;
	unsigned int getRenderedFrames() const;
private:
	int initWindow();
	void initAssimpLog();
	void cleanAssimpLog();
	bool pollEvent();
	void renderSceneAndChild();
	void cleanSceneAndChild();

	/* error checking/handling */
	void checkOpenGLError(std::string messageIfError) const;
};