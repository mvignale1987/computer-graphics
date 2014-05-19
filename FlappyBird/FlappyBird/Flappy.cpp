#include "Flappy.h"
#include "Vector3.h" 
#include "Bridge.h"
#include <GL/freeglut.h>

const float Flappy::initialHeight = 120;
const float Flappy::maxHeight = 210;
const float Flappy::gravity = -1000;
const float Flappy::gravity2 = -2;
const Vector3 Flappy::displacement(-40, 0, 0);
const float Flappy::velocityJump = 40;
const float Flappy::maxTrailDistance = 100;
const float Flappy::birdSize = 7;

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
	jumpedFirstTime(false),
	trailTexture("rainbow.png", false, true, 0)
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
	trailPoints.clear();
}

bool Flappy::isDead() const
{
	return !alive;
}

bool Flappy::heJumpedFirstTime() const
{
	return jumpedFirstTime;
}

Vector3 Flappy::getPosition() const 
{
	return height * Vector3::up + displacement;
}

Bridge *Flappy::getColliderBridge() const 
{
	return colliderBridge;
}

float Flappy::getHeight() const
{
	return height;
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
	
	// trail
	glPushAttrib(GL_ENABLE_BIT);
	{
		glDisable(GL_LIGHTING);
		// bird
		switch(parent.app().getOptions()->renderMode())
		{
		case SOLID_RENDER:
			glDisable(GL_TEXTURE_2D);
			break;
		case WIREFRAME_RENDER:
			glDisable(GL_TEXTURE_2D);
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			break;
		default:
			break;
		}
		
		drawTrail(parent);

		Vector3 position = getPosition();
		glPushMatrix();
		{
			glDisable(GL_TEXTURE_2D);
			glTranslate(position);
			glColor(Vector3::fromRGB(255, 0, 0));
			glRotate(angle, Vector3::forward);
			glutSolidCube(birdSize);	
		}
		glPopMatrix();
	}
	glPopAttrib();
}

void Flappy::drawTrail(Scene& parent)
{
	float multiplier = parent.app().getOptions()->speedMultiplier();
	float frameTime = parent.app().getFrameTime();

	if(alive && jumpedFirstTime)
	{
		float pointDisplacement = frameTime * Bridge::bridgeVelocity * multiplier;
		trailPoints.push_front(FlappyTrailPoint(height, pointDisplacement));
	}

	glColor(Vector3::one);
	glBindTexture(trailTexture);
	float totalDisplacement = 0;
	std::deque<FlappyTrailPoint>::iterator it;
	int nPoints = 0;

	glBegin(GL_QUAD_STRIP);
	{
		for(it = trailPoints.begin(); it != trailPoints.end() && totalDisplacement < maxTrailDistance; ++it)
		{
			totalDisplacement += it->displacement;
			Vector3 upper = (it->height + birdSize/2) * Vector3::up + Vector3::left * totalDisplacement +  displacement;
			Vector3 lower = (it->height - birdSize/2) * Vector3::up + Vector3::left * totalDisplacement +  displacement;
			if(it == trailPoints.begin())
			{
				glTexCoord2i(nPoints, 1);
				glVertex(upper);
				glTexCoord2i(nPoints, 0);
				glVertex(lower);
			} else {
				glTexCoord2i(nPoints, 0);
				glVertex(lower);
				glTexCoord2i(nPoints, 1);
				glVertex(upper);
			}
			nPoints++;
		}
		trailPoints.erase(it, trailPoints.end());
	}
	glEnd();
}


FlappyTrailPoint::FlappyTrailPoint(float h, float d):
	height(h), displacement(d)
{
}