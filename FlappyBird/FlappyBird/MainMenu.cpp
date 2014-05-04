#include "MainMenu.h"
#include "Vector3.h"
#include <GL/GLU.h>

void MainMenu::init()
{
	logo = Texture("logo.png");
	
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glClearColor(Vector3::fromRGB(1, 134, 149));
}

void MainMenu::renderLogo()
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(logo);
	glBegin(GL_QUADS);
	{
		glTexCoord2f(0, 0);
		glVertex3f(-0.6f, 0.6f, 0.3f);
		glTexCoord2f(0, 1);
		glVertex3f(-0.6f, 0.3f, 0.3f);
		glTexCoord2f(1, 1);
		glVertex3f(0.6f, 0.3f, 0.3f);
		glTexCoord2f(1, 0);
		glVertex3f(0.6f, 0.6f, 0.3f);
	}
	glEnd();
	glDisable(GL_TEXTURE_2D);
}

void MainMenu::reshape(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if (w <= h) 
		gluOrtho2D (
		-1.0, 1.0,
		-(GLfloat) h/(GLfloat) w, (GLfloat) h/(GLfloat) w
	);
	else 
		gluOrtho2D (
		-(GLfloat) w/(GLfloat) h, (GLfloat) w/(GLfloat) h,
		-1, 1
		); 
}

void MainMenu::render()
{
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	renderLogo();
}

std::string MainMenu::windowTitle()
{
	return "Flappy Bird";
}

std::string MainMenu::appIconPath()
{
	return "icon.png";
}