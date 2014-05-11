#include "Flappy.h"
#include "Vector3.h" 

Flappy::Flappy():
	texture("icon.png", true, true),
	animTime(0)
{
}

void Flappy::render(Scene &parent)
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
			{	//TEXTURE				//QUAD
				glTexCoord2f(0, 0);		glVertex2f  (0.3f, 0.6f);
				glTexCoord2f(0, 1);		glVertex2f  (0.3f, 0.5f);
				glTexCoord2f(1, 1);		glVertex2f  (0.4f, 0.5f);
				glTexCoord2f(1, 0);		glVertex2f  (0.4f, 0.6f);
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

}