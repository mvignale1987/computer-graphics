#include "Bridge.h"
#include <GL\GLU.h>

const float Bridge::modelLength = 1245;
const int Bridge::nBridges = 3;


Bridge::Bridge(Camera *c):
	model("bridge.dae"),
	camera(c),
	animTime(0)
{
	texturedDisplayList = glGenLists(3);
	glNewList (texturedDisplayList, GL_COMPILE);
	{
		render(TEXTURED_RENDER);
	}
	glEndList ();
	solidDisplayList = texturedDisplayList+1;
	glNewList (solidDisplayList, GL_COMPILE);
	{
		render(SOLID_RENDER);
	}
	glEndList ();
	wireframeDisplayList = texturedDisplayList+2;
	glNewList (wireframeDisplayList, GL_COMPILE);
	{
		render(WIREFRAME_RENDER);
	}
	glEndList ();
}

void Bridge::render(Scene &parent)
{
	glMatrixMode(GL_MODELVIEW);
	
	// Reset and transform the matrix.
	glLoadIdentity();
	gluLookAt(camera->getPosition(), camera->getCenter());
	
	glTranslate(600 * Vector3::backward);
	glTranslate(9 * Vector3::down);
	glRotate(180, Vector3::up);

	glPushAttrib(GL_ENABLE_BIT);
	glDisable(GL_TEXTURE_2D);

	glTranslate((animTime - modelLength) * Vector3::right);

	switch(parent.app().getOptions()->renderMode())
	{
	case TEXTURED_RENDER:
		glCallList(texturedDisplayList);
		break;
	case SOLID_RENDER:
		glCallList(solidDisplayList);
		break;
	case WIREFRAME_RENDER:
		glCallList(wireframeDisplayList);
		break;
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