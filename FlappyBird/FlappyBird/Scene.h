#pragma once

#include <SDL.h>
#include <SDL_opengl.h>
#include <string>
#include <vector>

class SceneObject;

class Scene {
private:
	SDL_Window *win;
	SDL_Renderer *ren;
	SDL_GLContext glContext;
	float frameTime;
	unsigned long nFrames;
	std::vector<SceneObject *> objects;
public:
	Scene();
	int run(int argc = 0, char **argv = NULL);
	
	/* getters */
	SDL_Window *getWindow() const;
	SDL_Renderer *getRenderer() const;
	SDL_GLContext getGlContext() const;
	float getFrameTime() const;
	unsigned int getRenderedFrames() const;

	/* child object management */
	void addObject(SceneObject *object);
	void removeObject(SceneObject *object);

	/* error checking/handling */
	void checkOpenGLError(std::string messageIfError) const;
	void logSDLError(const std::string &prefix) const;
	void logError(const std::string &message) const;

	/* logging */
	void log(const std::string &message) const;
protected:
	// optional methods
	virtual void init();
	virtual bool handleEvent(const SDL_Event& ev);
	virtual void reshape (int w, int h);
	virtual void render();
	virtual void clean();

	// window properties
	virtual std::string windowTitle() const;
	virtual std::string logFileName() const;
	virtual std::string appIconPath() const;

	virtual ~Scene();

private:
	int initWindow();
	void initAssimpLog();
	void cleanAssimpLog();
	bool pollEvent();
	void renderSceneAndChild();
	void cleanSceneAndChild();
};
