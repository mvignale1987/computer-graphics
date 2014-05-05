#pragma once
#include "Scene.h"
#include "Texture.h"
#include "Text.h"
#include "Cursor.h"
#include <SDL_ttf.h>

class MainMenu: public Scene
{
private:
	Texture logo;
	GLfloat logoAnimTime;
	Text	playText, optionsText, quitText,
			playTextHover, optionsTextHover, quitTextHover;
	Cursor	cursor;
public:
	MainMenu();
protected:
	void init();
	void reshape(int w, int h);
	void render();
	std::string windowTitle();
	std::string appIconPath();
private:
	void renderTexts();
};