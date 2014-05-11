#include "Flappy.h"
#include "Vector3.h" 

Flappy::Flappy():
	texture("icon.png", false, true),
	animTime(0)
{
}

void Flappy::render(Scene &parent)
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

		glPushAttrib(GL_ENABLE_BIT);
		glToggle(GL_TEXTURE_2D, parent.app().getOptions()->getTexturesEnabled());
		glBindTexture(texture);

		glBegin(GL_QUADS);
		{
			glColor3f    (1, 1, 1);
			glTexCoord2f (0, 3);
			glVertex3f   (-10, -3,  0);
			glTexCoord2f (0, 0);
			glVertex3f   (-10, -3, -2);
			glTexCoord2f (30, 0);
			glVertex3f   ( 10, -3, -2);
			glTexCoord2f (30, 3);
			glVertex3f   ( 10, -3,  0);
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