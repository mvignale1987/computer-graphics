#pragma once

#include "Scene.h"
#include "MainMenu.h"

class OptionsMenu: public Scene
{
private:
	MainMenu &mainMenu;
	TextHover *backText;
	Cursor	  *cursor;
	bool	  inited;
public:
	OptionsMenu(MainMenu& mainMenu);
	void init();
	void render();
private:
	void initFonts();
	void initCursor();
};