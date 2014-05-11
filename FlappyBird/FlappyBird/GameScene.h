#pragma once

#include "Scene.h"
#include "Flappy.h"
#include "Pipe.h"
#include "MainMenu.h"
class GameScene: public Scene
{
private:
	MainMenu		&mainMenu;
	Text			*integrantes,*MV,*IA,*NB;
	Flappy			*pajarito;
	Pipe			*tubo1,*tubo2;
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