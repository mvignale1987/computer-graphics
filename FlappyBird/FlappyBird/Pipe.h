#pragma once
#include "SceneObject.h"
#include "Texture.h"

class Pipe: public SceneObject
{
private:
	GLfloat animTime;
	bool backside;
public:
	Pipe();

	void render(Scene &parent);
};