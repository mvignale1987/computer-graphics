#include "Flappy.h"
#include "Vector3.h" 
#include <GL/freeglut.h>

const float Flappy::initialHeight = 120;
const float Flappy::maxHeight = 210;
const float Flappy::gravity = -1000;
const float Flappy::gravity2 = -2;
const Vector3 Flappy::displacement(-40, 0, 0);
const float Flappy::velocityJump = 60;

#define max(x,y) ((x)>(y)?x:y)

Flappy::Flappy(Bridge *colliderBridge):
	height(initialHeight),
	colliderBridge(colliderBridge),
	velocity(0),
	acceleration(0),
	acceleration2(0),
	angle(0),
	flappingSound(Mix_LoadWAV("jump.wav")),
	dieSound(Mix_LoadWAV("die.wav")),
	alive(true),
	jumpedFirstTime(false)
{
}

void Flappy::handleEvent(Scene &, const SDL_Event& ev)
{
	// space or left mouse button
	if(
		alive &&
		(
			ev.type == SDL_KEYDOWN && !ev.key.repeat && ev.key.keysym.scancode == SDL_SCANCODE_SPACE ||
			ev.type == SDL_MOUSEBUTTONDOWN && ev.button.button == SDL_BUTTON_LEFT
		)
	){
		// jumps
		jumpedFirstTime = true;
		acceleration2 = acceleration = 0;
		velocity = velocityJump;
		Mix_PlayChannel(-1, flappingSound, 0);
	}
}

void Flappy::kill()
{
	if(!alive)
		return;
	alive = false;
	Mix_PlayChannel(-1, dieSound, 0);
}

void Flappy::respawn()
{
	alive = true;
	velocity = acceleration = acceleration2 = angle = 0;
	height = initialHeight;
	jumpedFirstTime = false;
}

bool Flappy::isDead() const
{
	return !alive;
}

bool Flappy::heJumpedFirstTime() const
{
	return jumpedFirstTime;
}

void Flappy::render(Scene &parent)
{
	// recalculate height, accel velocity
	if(alive && jumpedFirstTime)
	{
		float frameTime = parent.app().getFrameTime();
		acceleration2 += gravity2 * frameTime;
		acceleration += acceleration2 * (-gravity) * frameTime;
		velocity += acceleration * frameTime;
		height += velocity  * frameTime;
		angle = atanf(velocity/ (2 * velocityJump)) * 180 / (GLfloat)M_PI;
	}

	float minHeight = colliderBridge->getStreetHeight();
	if(height > maxHeight)
	{
		velocity = acceleration = acceleration2 = 0;
		height = maxHeight;
	} if(height < minHeight)
	{
		velocity = acceleration = acceleration2 = 0;
		height = minHeight;
		kill();
	}

	glColor(Vector3::fromRGB(255, 0, 0));
	glPushMatrix();
	{
		glTranslate(height * Vector3::up +  displacement);
		glRotate(angle, Vector3::forward);
		glutSolidCube(7);
	}
	glPopMatrix();
}