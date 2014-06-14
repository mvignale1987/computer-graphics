#pragma once

#include <SDL.h>
#include <vector>
#include <string>

class Renderer;

class App
{
private:
	Renderer *renderer;
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

	/* renderer management */
	void setRenderer(Renderer *);
	Renderer *getRenderer();

	/* getters */
	SDL_Window *getWindow() const;
	SDL_Renderer *getRenderer() const;
	SDL_GLContext getGlContext() const;
	float getFrameTime() const;
	unsigned int getRenderedFrames() const;

	/* manejo de vsync */
	void setVSync(bool enabled);

	/* error checking/handling */
	static void checkOpenGLError(std::string messageIfError);

private:
	int initWindow();
	void initAssimpLog();
	void cleanAssimpLog();
	bool pollEvent();
	void renderNextFrame();
	void clean();
};