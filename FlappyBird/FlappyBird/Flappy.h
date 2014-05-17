#pragma once
#include "SceneObject.h"
#include "Texture.h"
#include "Vector3.h"
#include "Bridge.h"

class Flappy: public SceneObject
{
private:
	static const float maxHeight;
	static const Vector3 displacement; // bird displacement from the center
	static const float gravity; 
	static const float gravity2; 
	static const float velocityJump;

	Bridge *colliderBridge;
	GLfloat height;
	GLfloat velocity;
	GLfloat acceleration;
	GLfloat acceleration2;
public:
	Flappy(Bridge *colliderBridge);

	void render(Scene &parent);
	void handleEvent(Scene &parent, const SDL_Event& ev);
};