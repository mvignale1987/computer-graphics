#pragma once

#include "Scene.h"
#include "MainMenu.h"
class EditorMenu: public Scene
{
private:
	MainMenu		&mainMenu;
	TextHover		*backText;
	Cursor			*cursor;
	Text			*integrantes,*MV,*IA,*NB;
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