#include "Floor.h"
#include "Vector3.h"

Floor::Floor():
	texture("green-pattern.jpg", false, true),
	animTime(0)
{
}

void Floor::render(Scene &parent)
{
	glMatrixMode(GL_TEXTURE);
	glPushMatrix();
	glLoadIdentity();
	glTranslatef(animTime, 0, 0);
	glMatrixMode(GL_MODELVIEW);
	

	glPushMatrix();
	{
		glTranslate(Vector3::backward * 4);
		glTranslate(Vector3::up);

		GLboolean texturesEnabled = glIsEnabled(GL_TEXTURE_2D);
		glToggle(GL_TEXTURE_2D, parent.app().getOptions()->getTexturesEnabled());
		glBindTexture(texture);

		glBegin(GL_QUADS);
		{
			glColor3f(1, 1, 1);
			glTexCoord2f(0, 1);
			glVertex3f(-10, -3,  0);
			glTexCoord2f(0, 0);
			glVertex3f(-10, -3, -2);
			glTexCoord2f(10, 0);
			glVertex3f( 10, -3, -2);
			glTexCoord2f(10, 1);
			glVertex3f( 10, -3,  0);
		}
		glEnd();
		glToggle(GL_TEXTURE_2D, texturesEnabled);
	}
	glPopMatrix();

	glMatrixMode(GL_TEXTURE);
	glPopMatrix();	
	glMatrixMode(GL_MODELVIEW);

	float multiplier = parent.app().getOptions()->speedMultiplier();
	

	animTime += parent.app().getFrameTime() * 0.6f * multiplier;
	if(animTime > 10)
		animTime -= 10;
}