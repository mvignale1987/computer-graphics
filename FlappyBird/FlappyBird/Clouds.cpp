#include "Clouds.h"
#include "Vector3.h"

Clouds::Clouds():
	texture("clouds.png", true, true),
	animTime(0)
{
}

void Clouds::render(Scene &parent)
{
	// pop texture animation
	glMatrixMode(GL_TEXTURE);
	glPushMatrix();
	glLoadIdentity();
	glTranslatef(animTime, 0, 0);
	glMatrixMode(GL_MODELVIEW);	

	glPushAttrib(GL_ENABLE_BIT);
	glDisable(GL_DEPTH_TEST);
	glToggle(GL_TEXTURE_2D, parent.app().getOptions()->getTexturesEnabled());

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	{
		glLoadIdentity();
		glOrtho(0, 1, 0, 1, -1, 1);
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		{
			glLoadIdentity();
			glBindTexture(texture);
			glBegin(GL_QUADS);
			{
				glTexCoord2f(0, 0);
				glVertex2f  (0, 0.98f);
				glTexCoord2f(0, 1);
				glVertex2f  (0, 0.6f);
				glTexCoord2f(1, 1);
				glVertex2f  (1, 0.6f);
				glTexCoord2f(1, 0);
				glVertex2f  (1, 0.98f);
			}
			glEnd();
		}
		glPopMatrix();
	}

	// restore matrices
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_TEXTURE);
	glPopMatrix();	
	glMatrixMode(GL_MODELVIEW);

	glPopAttrib();

	float multiplier = parent.app().getOptions()->speedMultiplier();
	

	animTime += parent.app().getFrameTime() * 0.01f * multiplier;
	if(animTime > 10)
		animTime -= 10;
}