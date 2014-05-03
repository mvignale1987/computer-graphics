#pragma once

#include <SDL.h>
#include <SDL_opengl.h>
#include <string>

class Scene {
private:
	SDL_Window *win;
	SDL_Renderer *ren;
	SDL_GLContext glContext;
	float frameTime;
	unsigned long nFrames;
public:
	Scene();
	int run(int argc = 0, char **argv = NULL);
	
	/* getters */
	SDL_Window *getWindow() const;
	SDL_Renderer *getRenderer() const;
	SDL_GLContext getGlContext() const;
	float getFrameTime() const;
	unsigned int getRenderedFrames() const;

	/* error checking/handling */
	void checkOpenGLError(std::string messageIfError);
	void logSDLError(const std::string &prefix);
	void logError(const std::string &message);

	/* logging */
	void log(const std::string &message);
protected:
	virtual void init();
	virtual bool handleEvent(SDL_Event ev);
	virtual void reshape (int w, int h);
	virtual void render();
	virtual void clean();
	virtual std::string windowTitle();
	virtual std::string logFileName();
	virtual std::string appIconPath();

	virtual ~Scene();

private:
	int initWindow();
	void initAssimpLog();
	void cleanAssimpLog();
	bool pollEvent();
};
