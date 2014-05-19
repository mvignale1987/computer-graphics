#include "Pipe.h"
#include "Vector3.h"
#include <GL/freeglut.h>

const float Pipe::aperture = 35.0f;
const float Pipe::upperPipeLength = 400.0f;
const float Pipe::ratio = 10.0f;
const int   Pipe::slices = 16;
const int   Pipe::stacks = 4;


Pipe::Pipe(Bridge& colliderBridge, float initialPosition, float apertureHeight):
	colliderBridge(colliderBridge),
	position(initialPosition + Flappy::displacement.x()),
	apertureHeight(apertureHeight),
	stopped(false)
{

}

float Pipe::getAbsolutePosition() const
{
	return position - Flappy::displacement.x();
}

void Pipe::stop()
{
	stopped = true;
}

void Pipe::render(Scene &parent)
{
	float lowerTubeOrigin = colliderBridge.getStreetHeight(position) + apertureHeight - 5;

	float multiplier = parent.app().getOptions()->speedMultiplier();
	float frameTime = parent.app().getFrameTime();

	glPushAttrib(GL_ENABLE_BIT);
	{
		glDisable(GL_TEXTURE_2D);
		glEnable(GL_LIGHTING);
		glColor(Vector3::fromRGB(82,130,33));
		glPushMatrix();
		{
			glTranslate(Vector3::up * lowerTubeOrigin);
			glTranslate(Vector3::right * position);
			glRotate(90, Vector3::right);
			glutSolidCylinder(ratio, apertureHeight, slices, stacks);
		}
		glPopMatrix();

		glPushMatrix();
		{
			glTranslate(Vector3::up * (lowerTubeOrigin + upperPipeLength + aperture) );
			glTranslate(Vector3::right * position);
			glRotate(90, Vector3::right);
			glutSolidCylinder(ratio, upperPipeLength, slices, stacks);
		}
		glPopMatrix();
	}
	glPopAttrib();

	if(!stopped)
	{
		position -=  frameTime * Bridge::bridgeVelocity * multiplier;
	}
}

bool Pipe::collidesWith(Flappy& flappy) const
{
	float midBirdSize = Flappy::birdSize / 2;

	if(beforePipe())
		return false;
	else if(pastPipe())
		return false;
	else {
		float lowerTubeOrigin = colliderBridge.getStreetHeight(position) + apertureHeight - 5;
		float upperTubeOrigin = lowerTubeOrigin + aperture;
		float height = flappy.getHeight();
		if(height - midBirdSize < lowerTubeOrigin || height + midBirdSize > upperTubeOrigin)
			return true;
		else
			return false;
	}
}

bool Pipe::pastMiddle() const
{
	return position - Flappy::displacement.x()  < Flappy::birdSize / 2;
}

bool Pipe::beforePipe() const
{
	return position - Flappy::displacement.x() - ratio >  Flappy::birdSize / 2;
}

bool Pipe::pastPipe() const
{
	return position - Flappy::displacement.x()  + ratio < Flappy::birdSize / 2;
}