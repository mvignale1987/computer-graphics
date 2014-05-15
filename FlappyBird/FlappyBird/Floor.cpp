#include "Floor.h"
#include "Vector3.h"
#include <GL/GLU.h>

Floor::Floor(Camera *c):
	texture("green-pattern.jpg", false, true, 3),
	animTime(0),
	camera(c)
{
}

void Floor::render(Scene &parent)
{
	glMatrixMode(GL_TEXTURE);
	glPushMatrix();
	glLoadIdentity();
	glTranslatef(-animTime, 0, 0);
	glMatrixMode(GL_MODELVIEW);
	
	// Reset and transform the matrix.
	glLoadIdentity();
	gluLookAt(
		camera->x(), camera->y(), camera->z(),
		0,0,0,
		0,1,0
		);

	glPushMatrix();
	{
		glPushAttrib(GL_ENABLE_BIT);
		glToggle(GL_TEXTURE_2D, parent.app().getOptions()->getTexturesEnabled());
		glBindTexture(texture);

		const float floorSize = 1000;

		glBegin(GL_QUADS);
		{
			//glColor(Vector3::fromRGB(86, 157, 223));
			glColor(Vector3::one);
			glTexCoord2f (0, floorSize/10); glVertex3f   (-floorSize/2, -10,  floorSize/2);
			glTexCoord2f (0, 0);  glVertex3f   (-floorSize/2, -10, -floorSize/2);
			glTexCoord2f (floorSize/10, 0); glVertex3f   ( floorSize/2, -10, -floorSize/2);
			glTexCoord2f (floorSize/10, floorSize/10); glVertex3f   ( floorSize/2, -10,  floorSize/2);
		}
		glEnd();
		glPopAttrib();
	}
	glPopMatrix();

	glMatrixMode(GL_TEXTURE);
	glPopMatrix();	
	glMatrixMode(GL_MODELVIEW);

	float multiplier = parent.app().getOptions()->speedMultiplier();
	

	animTime += parent.app().getFrameTime() * 1.8f * multiplier;
	if(animTime > 10)
		animTime -= 10;
}