#include "MainMenu.h"
#include "Vector3.h"
#include "SceneError.h"
#include <GL/GLU.h>

MainMenu::MainMenu()
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

	// init fonts
	TTF_Font *font = TTF_OpenFont("Flappy.ttf", 42);
	if (font == NULL){
		throw SceneError::fromSDLError("Couldn't load font: TTF_OpenFont");
    }
	Vector3 white = Vector3::fromRGB(255,255,255);
	playText = Text("Play", font, white, CENTER, 0, 0);
	optionsText = Text("Options", font, white, CENTER, 0, 50);
	quitText = Text("Quit", font, white, CENTER, 0, 100);
	TTF_CloseFont(font);

	TTF_Font *fontHover = TTF_OpenFont("Flappy.ttf", 46);
	if (fontHover == NULL){
		throw SceneError::fromSDLError("Couldn't load font: TTF_OpenFont");
    }
	Vector3 yellow = Vector3::fromRGB(236, 218, 19);
	playTextHover = Text("Play", fontHover, yellow, CENTER, 0, 0);
	optionsTextHover = Text("Options", fontHover, yellow, CENTER, 0, 50);
	quitTextHover = Text("Quit", fontHover, yellow, CENTER, 0, 100);
	TTF_CloseFont(fontHover);

	// init cursor
	SDL_ShowCursor(0);
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

	if(playTextHover.mouseHover(getWindow())){
		playTextHover.render(getWindow());
	} else {
		playText.render(getWindow());
	}
	if(optionsText.mouseHover(getWindow())){
		optionsTextHover.render(getWindow());
	} else {
		optionsText.render(getWindow());
	}
	if(quitText.mouseHover(getWindow())){
		quitTextHover.render(getWindow());
	} else {
		quitText.render(getWindow());
	}

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