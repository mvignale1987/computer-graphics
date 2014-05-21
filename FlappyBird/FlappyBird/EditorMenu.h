#pragma once

#include "Scene.h"
#include "Pipe.h"
#include "MainMenu.h"
class EditorMenu: public Scene
{
private:
	MainMenu		&mainMenu;
	TextHover		*backText;
	Cursor			*cursor;
	Pipe			*tubo;
	bool			inited;
public:
	EditorMenu(MainMenu& mainMenu);
	void init();
	void reshape(int, int);
	bool handleEvent(const SDL_Event& ev);
	void render();
private:
	void initFonts();
	void initCursor();
};