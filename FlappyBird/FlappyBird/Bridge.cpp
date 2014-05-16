#include "Bridge.h"
#include <GL\GLU.h>

const float Bridge::modelLength = 1245;
const int Bridge::nBridges = 3;


Bridge::Bridge(Camera *c):
	model("bridge.dae"),
	camera(c),
	animTime(0)
{
	texturedDisplayList = glGenLists(1);
	glNewList (texturedDisplayList, GL_COMPILE);
	{
		render(TEXTURED_RENDER);
	}
	glEndList ();
	solidDisplayList = glGenLists(1);
	glNewList (solidDisplayList, GL_COMPILE);
	{
		render(SOLID_RENDER);
	}
	glEndList ();
}

void Bridge::render(Scene &parent)
{
	glMatrixMode(GL_MODELVIEW);
	
	// Reset and transform the matrix.
	glLoadIdentity();
	gluLookAt(camera->getPosition(), camera->getCenter());
	
	glTranslate(500 * Vector3::backward);
	glTranslate(9 * Vector3::down);
	glRotate(180, Vector3::up);

	glPushAttrib(GL_ENABLE_BIT);
	glDisable(GL_TEXTURE_2D);

	glTranslate((animTime + modelLength) * Vector3::left);

	if(parent.app().getOptions()->renderMode() == TEXTURED_RENDER)
	{
		glCallList(texturedDisplayList);
	} else {
		glCallList(solidDisplayList);
	}

	float multiplier = parent.app().getOptions()->speedMultiplier();
	animTime +=  parent.app().getFrameTime() * 15 * multiplier;
	if(animTime > modelLength)
		animTime -= modelLength;

	glPopAttrib();
}

void Bridge::render(RenderMode mode)
{
	for(int i = 0; i < nBridges; ++ i)
	{
		model.render(6, mode);
		glTranslate(modelLength * Vector3::right);
	}
}