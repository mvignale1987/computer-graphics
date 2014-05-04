#include "MainMenu.h"
#include "Vector3.h"
#include <GL/GLU.h>

void MainMenu::init()
{
	logoAnimTime = 0;
	logo = Texture("logo.png");
	
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glClearColor(Vector3::fromRGB(1, 134, 149));
}

void MainMenu::renderLogo()
{
	const GLfloat width = 0.5f;
	const GLfloat height = 0.134f;

	glEnable(GL_TEXTURE_2D);
	glBindTexture(logo);
	glBegin(GL_QUADS);
	{
		glTexCoord2f(0, 0);
		glVertex2f(-width, height);
		glTexCoord2f(0, 1);
		glVertex2f(-width, -height);
		glTexCoord2f(1, 1);
		glVertex2f(width, -height);
		glTexCoord2f(1, 0);
		glVertex2f(width, height);
	}
	glEnd();
	glDisable(GL_TEXTURE_2D);
}

void MainMenu::reshape(int width, int height)
{
	if (height==0)
	{
		height=1;
	}
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f,(GLfloat)width/(GLfloat)height, 0.1f, 100.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void MainMenu::render()
{
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	float angle = sinf(logoAnimTime * 2 * (GLfloat) M_PI) * 6.0f;
	float offsetDistance = sinf(logoAnimTime * 3.2f * (GLfloat) M_PI) * 0.07f;

	glTranslate(Vector3::up * 0.4f);
	glTranslate(Vector3::backward * (2 + offsetDistance));
	glRotate(angle, Vector3::forward);
	renderLogo();

	logoAnimTime += getFrameTime() * 0.05f;
	if(logoAnimTime > 1)
		logoAnimTime -= 1;
}

std::string MainMenu::windowTitle()
{
	return "FlappyBird";
}

std::string MainMenu::appIconPath()
{
	return "icon.png";
}