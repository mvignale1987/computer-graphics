#include "Bridge.h"
#include <GL\GLU.h>


Bridge::Bridge(Camera *c):
	model("bridge.dae"),
	camera(c)
{
}

void Bridge::render(Scene &)
{
	glMatrixMode(GL_MODELVIEW);
	
	// Reset and transform the matrix.
	glLoadIdentity();
	gluLookAt(camera->getPosition(), camera->getCenter());
	
	glTranslate(500 * Vector3::backward);
	glTranslate(9 * Vector3::down);
	glRotate(180, Vector3::up);

	const float modelLength = 1245;
	const float visibleLength = 1000;
	const int nBridges = (float) ceilf(visibleLength / modelLength);

	glPushAttrib(GL_ENABLE_BIT);
	glDisable(GL_TEXTURE_2D);

	for(int i = 0; i < 2; ++ i)
	{
		model.render(6);
		glTranslate(modelLength * Vector3::right);
	}

	glPopAttrib();
}