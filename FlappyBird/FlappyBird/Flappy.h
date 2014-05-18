#pragma once
#include "SceneObject.h"
#include "Texture.h"
#include "Vector3.h"
#include "Bridge.h"
#include <SDL_mixer.h>
#include <deque>

class FlappyTrailPoint
{
public:
	FlappyTrailPoint(float h, float d);

	float height;
	float displacement;
};

class Flappy: public SceneObject
{
private:
	static const float initialHeight;
	static const float maxHeight;
	static const Vector3 displacement; // bird displacement from the center
	static const float gravity; 
	static const float gravity2; 
	static const float velocityJump;
	static const float maxTrailDistance;
	static const float birdSize;

	Bridge *colliderBridge;
	// positioning
	GLfloat height;
	GLfloat velocity;
	GLfloat acceleration;
	GLfloat acceleration2;
	GLfloat angle;
	// sounds
	Mix_Chunk *flappingSound;
	Mix_Chunk *dieSound;

	bool alive;
	bool jumpedFirstTime;

	// trail
	std::deque<FlappyTrailPoint> trailPoints;
	Texture trailTexture;
public:
	Flappy(Bridge *colliderBridge);

	void kill();
	void respawn();
	bool isDead() const;
	bool heJumpedFirstTime() const;
	void render(Scene &parent);
	void handleEvent(Scene &parent, const SDL_Event& ev);
private:
	void drawTrail(Scene &parent);
};
