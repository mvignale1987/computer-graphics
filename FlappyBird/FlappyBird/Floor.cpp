#include "Floor.h"
#include "Vector3.h"

Floor::Floor(Camera *c):
	texture("green-pattern.jpg", false, true, 3),
	animTime(0),
	camera(c)
{
	texturedDisplayList = glGenLists(1);
	glNewList (texturedDisplayList, GL_COMPILE);
	{
		render(TEXTURED_RENDER);
	}
	glEndList();
	solidDisplayList = glGenLists(1);
	glNewList (solidDisplayList, GL_COMPILE);
	{
		render(SOLID_RENDER);
	}
	glEndList();
}

void Floor::render(RenderMode mode)
{
	glPushMatrix();
	{
		glPushAttrib(GL_ENABLE_BIT);

		if(mode == TEXTURED_RENDER)
		{
			glEnable(GL_TEXTURE_2D);
			glBindTexture(texture);
		} else {
			glDisable(GL_TEXTURE_2D);
		}

		const float floorSize = 4000;

		glBegin(GL_QUADS);
		{
			if(mode == TEXTURED_RENDER)
			{
				glColor(Vector3::one);
			} else {
				glColor(Vector3::fromRGB(96, 151, 50));
			}
			
			glTexCoord2f (0, floorSize/10); glVertex3f   (-floorSize/2, 0,  floorSize/2);
			glTexCoord2f (0, 0);  glVertex3f   (-floorSize/2, 0, -floorSize/2);
			glTexCoord2f (floorSize/10, 0); glVertex3f   ( floorSize/2, 0, -floorSize/2);
			glTexCoord2f (floorSize/10, floorSize/10); glVertex3f   ( floorSize/2, 0,  floorSize/2);
		}
		glEnd();
		glPopAttrib();
	}
	glPopMatrix();
}

void Floor::render(Scene &parent)
{	
	if(parent.app().getOptions()->renderMode() != WIREFRAME_RENDER)
	{
		glLoadIdentity();
		gluLookAt(camera->getPosition(), camera->getCenter());

		switch(parent.app().getOptions()->renderMode())
		{
		case TEXTURED_RENDER:
			glMatrixMode(GL_TEXTURE);
			glPushMatrix();
			glLoadIdentity();
			glTranslatef(-animTime, 0, 0);
			glMatrixMode(GL_MODELVIEW);
			glCallList(texturedDisplayList);
			glMatrixMode(GL_TEXTURE);
			glPopMatrix();	
			glMatrixMode(GL_MODELVIEW);
			break;
		case SOLID_RENDER:
			glCallList(solidDisplayList);
			break;
		default:
			break;
		}
	}

	float multiplier = parent.app().getOptions()->speedMultiplier();
	animTime += parent.app().getFrameTime() * 1.8f * multiplier;
	if(animTime > 10)
		animTime -= 10;
}