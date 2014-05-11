#include "Pipe.h"
#include "Vector3.h" 

Pipe::Pipe(bool b):
	texture("pipe.png", true, true),
	animTime(0)
{
	backside = b;
}

void Pipe::render(Scene &parent)
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
			{	//TEXTURE								//QUAD INF							//QUAD SUP
				glTexCoord2f(0, 0);		if(backside)	glVertex2f(0.8f, 0.3f);		else	glVertex2f(0.8f, 0.7f);
				glTexCoord2f(0, 1);		if(backside)	glVertex2f(0.8f, 0);		else	glVertex2f(0.8f, 1.0f);	
				glTexCoord2f(1, 1);		if(backside)	glVertex2f(1.0f, 0);		else	glVertex2f(1.0f, 1.0f);
				glTexCoord2f(1, 0);		if(backside)	glVertex2f(1.0f, 0.3f);		else	glVertex2f(1.0f, 0.7f);
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