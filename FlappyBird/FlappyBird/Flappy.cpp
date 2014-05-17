#include "Flappy.h"
#include "Vector3.h" 
#include <GL/freeglut.h>

const float Flappy::maxHeight = 210;
const float Flappy::gravity = -1000;
const float Flappy::gravity2 = -2;
const Vector3 Flappy::displacement(-40, 0, 0);
const float Flappy::velocityJump = 60;

#define max(x,y) ((x)>(y)?x:y)

Flappy::Flappy(Bridge *colliderBridge):
	height(120),
	colliderBridge(colliderBridge),
	velocity(0),
	acceleration(0),
	acceleration2(0)
{
}

void Flappy::handleEvent(Scene &, const SDL_Event& ev)
{
	// space or left mouse button
	if(ev.type == SDL_KEYDOWN && !ev.key.repeat && ev.key.keysym.scancode == SDL_SCANCODE_SPACE ||
		ev.type == SDL_MOUSEBUTTONDOWN && ev.button.button == SDL_BUTTON_LEFT)
	{
		acceleration2 = acceleration = 0;
		velocity = velocityJump;
	}
}


void Flappy::render(Scene &parent)
{
	// recalculate height, accel velocity
	float frameTime = parent.app().getFrameTime();
	acceleration2 += gravity2 * frameTime;
	acceleration += acceleration2 * (-gravity) * frameTime;
	velocity += acceleration * frameTime;
	height += velocity  * frameTime;


	float minHeight = colliderBridge->getStreetHeight();
	if(height > maxHeight)
	{
		velocity = acceleration = acceleration2 = 0;
		height = maxHeight;
	} if(height < minHeight)
	{
		velocity = acceleration = acceleration2 = 0;
		height = minHeight;
	}

	glColor(Vector3::fromRGB(255, 0, 0));
	glPushMatrix();
	{
		glTranslate(height * Vector3::up +  displacement);
		glRotate(atanf(velocity/ (2 * velocityJump)) * 180 / (GLfloat)M_PI, Vector3::forward);
		glutSolidCube(7);
	}
	glPopMatrix();
}