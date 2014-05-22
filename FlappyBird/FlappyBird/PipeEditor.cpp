#include "PipeEditor.h"
#include "Vector3.h"
#include <GL/freeglut.h>

const float PipeEditor::aperture = 35.0f;
const float PipeEditor::upperPipeLength = 400.0f;
const float PipeEditor::ratio = 8.0f;
const int   PipeEditor::slices = 16;
const int   PipeEditor::stacks = 1;


PipeEditor::PipeEditor(Bridge& colliderBridge, float initialPosition, float apertureHeight):
	colliderBridge(colliderBridge),
	position(initialPosition + Flappy::displacement.x()),
	apertureHeight(apertureHeight),
	stopped(false)
{

}

void PipeEditor::pushCurrentApertureHeight()
{
	pipeSizes.push_back(apertureHeight);
}



void PipeEditor::render(Scene &parent)
{
	float lowerTubeOrigin = colliderBridge.getStreetHeight(position) + apertureHeight - 5;

	float multiplier = parent.app().getOptions()->speedMultiplier();
	float frameTime = parent.app().getFrameTime();


	int width, height;
	SDL_GetWindowSize(parent.app().getWindow(), &width, &height);
	int mouseX, mouseY;
	SDL_GetMouseState(&mouseX, &mouseY);

	//int left = mouseX + offsetX;
	//int right = left + texture.width();
	//int top = height - mouseY - offsetY;
	//int bottom = top - texture.height();
	apertureHeight = height - mouseY*2; 
	glMaterial(GL_FRONT_AND_BACK, GL_DIFFUSE, Vector3::fromRGB(0, 200, 0));

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
	float positionIt = position;
	for(std::vector<float>::iterator it = pipeSizes.begin(); it != pipeSizes.end(); ++it)
	{
		positionIt -= 20;
		glPushMatrix();
		{
			glTranslate(Vector3::up * lowerTubeOrigin);
			glTranslate(Vector3::right * positionIt);
			glRotate(90, Vector3::right);
			glutSolidCylinder(ratio, apertureHeight, slices, stacks);
		}
		glPopMatrix();

		glPushMatrix();
		{
			glTranslate(Vector3::up * (lowerTubeOrigin + upperPipeLength + aperture) );
			glTranslate(Vector3::right * positionIt);
			glRotate(90, Vector3::right);
			glutSolidCylinder(ratio, upperPipeLength, slices, stacks);
		}
		glPopMatrix();
	}
	glPopAttrib();


}

