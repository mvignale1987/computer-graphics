#include "Bridge.h"
#include <GL\GLU.h>

const float Bridge::modelLength = 1245;
const int Bridge::nBridges = 3;


Bridge::Bridge(Camera *c):
	model("bridge.dae"),
	camera(c),
	animTime(0)
{
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

	for(int i = 0; i < nBridges; ++ i)
	{
		model.render(6);
		glTranslate(modelLength * Vector3::right);
	}

	float multiplier = parent.app().getOptions()->speedMultiplier();
	animTime +=  parent.app().getFrameTime() * 15 * multiplier;
	if(animTime > modelLength)
		animTime -= modelLength;

	glPopAttrib();
}