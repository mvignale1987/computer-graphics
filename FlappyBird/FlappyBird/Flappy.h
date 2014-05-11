#pragma once
#include "SceneObject.h"
#include "Texture.h"

class Flappy: public SceneObject
{
private:
	Texture texture;
	GLfloat animTime;
public:
	Flappy();

	void render(Scene &parent);
};