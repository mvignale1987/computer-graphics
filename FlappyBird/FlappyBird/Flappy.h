#pragma once
#include "SceneObject.h"
#include "Texture.h"
#include "Vector3.h"
#include "Bridge.h"
#include <SDL_mixer.h>

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
	Mix_Chunk *flappingSound;
	Mix_Chunk *dieSound;

	bool alive;
	bool jumpedFirstTime;
public:
	Flappy(Bridge *colliderBridge);

	void kill();
	bool isDead();
	void render(Scene &parent);
	void handleEvent(Scene &parent, const SDL_Event& ev);
};