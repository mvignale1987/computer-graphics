#pragma once

#include "SceneObject.h"
#include "Texture.h"

class Floor: public SceneObject
{
private:
	Texture texture;
	GLfloat animTime;
public:
	Floor();

	void render(Scene &parent);
};