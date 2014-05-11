#pragma once
#include "SceneObject.h"
#include "Texture.h"

class Pipe: public SceneObject
{
private:
	Texture texture;
	GLfloat animTime;
	bool backside;
public:
	Pipe(bool b);

	void render(Scene &parent);
};