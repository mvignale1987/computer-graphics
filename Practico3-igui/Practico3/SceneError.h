#ifndef SCENE_ERROR_H
#define SCENE_ERROR_H

#include <exception>
#include <string>
#include <SDL_opengl.h>

using namespace std;

class SceneError: public exception
{
public:
	SceneError(const char *msg);
	static SceneError fromSDLError(const string &msg);
	static SceneError fromGLError(GLenum glGetErrorRes, const string &msg);
	virtual ~SceneError();
};

#endif