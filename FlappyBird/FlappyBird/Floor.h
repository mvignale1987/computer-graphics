#pragma once

#include "SceneObject.h"
#include "Texture.h"
#include "Camera.h"



class Floor: public SceneObject
{
private:
	Texture texture;
	GLfloat animTime;
	Camera  *camera;
	GLuint	texturedDisplayList, solidDisplayList, wireframeDisplayList;
public:
	Floor(Camera *c);

	void render(Scene &parent);
	void render(RenderMode mode);
};