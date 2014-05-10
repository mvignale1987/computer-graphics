#pragma once

#include "SceneObject.h"
#include "Texture.h"

class Clouds: public SceneObject
{
private:
	Texture texture;
	GLfloat animTime;
public:
	Clouds();

	void render(Scene &parent);
};