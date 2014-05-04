#include "MainMenu.h"
#include "Vector3.h"
#include "SceneError.h"
#include <GL/GLU.h>

MainMenu::MainMenu():
	font(NULL)
{
}

void MainMenu::init()
{
	logoAnimTime = 0;
	logo = Texture("logo.png");
	
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glClearColor(Vector3::fromRGB(1, 134, 149));
	glEnable(GL_TEXTURE_2D);

	font = TTF_OpenFont("Flappy.ttf", 42);
	if (font == NULL){
		throw SceneError::fromSDLError("Couldn't load font: TTF_OpenFont");
    }

	playText = Text("Play", font, Vector3::one, CENTER, 0, 0);
	optionsText = Text("Options", font, Vector3::one, CENTER, 0, 50);
	quitText = Text("Quit", font, Vector3::one, CENTER, 0, 100);

	//SDL_ShowCursor(0);
	cursor = Cursor("cursor.png", -4, -2);
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
	float offsetDistance = sinf(logoAnimTime * 3 * (GLfloat) M_PI) * 0.15f;

	glPushMatrix();
	{
		glTranslate(Vector3::up);
		glTranslate(Vector3::backward * (4 + offsetDistance));
		glRotate(angle, Vector3::forward);
		logo.render();
	}
	glPopMatrix();

	playText.render(getWindow());
	optionsText.render(getWindow());
	quitText.render(getWindow());

	cursor.render(getWindow());

	logoAnimTime += getFrameTime() * 0.05f;
	if(logoAnimTime > 6)
		logoAnimTime -= 6;
}

std::string MainMenu::windowTitle()
{
	return "FlappyBird";
}

std::string MainMenu::appIconPath()
{
	return "icon.png";
}