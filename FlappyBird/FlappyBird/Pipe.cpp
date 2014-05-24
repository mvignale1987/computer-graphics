#include "Pipe.h"
#include "Vector3.h"
#include <GL/freeglut.h>

const float Pipe::aperture = 35.0f;
const float Pipe::upperPipeLength = 400.0f;
const float Pipe::ratio = 8.0f;
const int   Pipe::slices = 8;
const int   Pipe::stacks = 1;
Model *		Pipe::pipeEnd = NULL;


Pipe::Pipe(Bridge& colliderBridge, float initialPosition, float apertureHeight):
	colliderBridge(colliderBridge),
	position(initialPosition + Flappy::displacement.x()),
	apertureHeight(apertureHeight),
	stopped(false)
{
	if(pipeEnd == NULL)
	{
		pipeEnd = new Model("pipeEnd.obj");
	}
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

	RenderMode renderMode = parent.app().getOptions()->renderMode();

	glMaterial(GL_FRONT_AND_BACK, GL_DIFFUSE, Vector3::fromRGB(0, 200, 0));

	glPushAttrib(GL_ENABLE_BIT);
	{
		glDisable(GL_TEXTURE_2D);
		glEnable(GL_LIGHTING);
		glColor(Vector3::fromRGB(82,130,33));
		glMaterial(GL_DIFFUSE, Vector3::fromRGB(82,130,33), 1);
		glMaterial(GL_SPECULAR, Vector3::zero, 0);
		glMaterial(GL_AMBIENT, Vector3::fromRGB(21,34,9), 1);
		glMaterial(GL_EMISSION, Vector3::zero, 0);
		glPushMatrix();
		{
			glTranslate(Vector3::up * lowerTubeOrigin);
			glTranslate(Vector3::right * position);
			glRotate(90, Vector3::right);

			(renderMode == WIREFRAME_RENDER ? glutWireCylinder : glutSolidCylinder)(ratio, apertureHeight, slices, stacks);
			/*glRotate(-90, Vector3::right);
			glScalef(ratio*1.1f,ratio*1.1f,ratio*1.1f);
			pipeEnd->render(1,  renderMode);*/
		}
		glPopMatrix();

		glPushMatrix();
		{
			glTranslate(Vector3::up * (lowerTubeOrigin + upperPipeLength + aperture) );
			glTranslate(Vector3::right * position);
			glRotate(90, Vector3::right);

			(renderMode == WIREFRAME_RENDER ? glutWireCylinder : glutSolidCylinder)(ratio, upperPipeLength, slices, stacks);
			/*glRotate(90, Vector3::right);
			glScalef(ratio*1.1f,ratio*1.1f,ratio*1.1f);
			pipeEnd->render(1,  renderMode);*/
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