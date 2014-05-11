#pragma once

#include "Scene.h"
#include "MainMenu.h"
class GameScene: public Scene
{
private:
	MainMenu		&mainMenu;
	Text			*integrantes,*MV,*IA,*NB;
	bool			inited;
public:
	GameScene(MainMenu& mainMenu);
	void init();
	void reshape(int, int);
	bool handleEvent(const SDL_Event& ev);
	void render();
private:
	void initFonts();
};