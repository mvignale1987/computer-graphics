#pragma once

#include "Scene.h"
#include "Flappy.h"
#include "Pipe.h"
#include "MainMenu.h"
#include "FlappyFeatherParticleSystem.h"
#include "PipeLevel.h"

enum GameSceneStatus { GAME_SCENE_GET_READY, GAME_SCENE_PLAYING, GAME_SCENE_GAME_OVER, GAME_SCENE_FADING_OUT };

class GameScene: public Scene
{
private:
	MainMenu					&mainMenu;
	TTF_Font					*scoreFont;
	TextOptions					scoreTextOptions;
	Text						*scoreText, *getReadyText, *gameOverText;
	FadeInOut					*dieEffect;
	Flappy						*flappy;
	FlappyFeatherParticleSystem	*dieParticleSystem;
	PipeLevel					*level;
	Mix_Chunk					*pointSound;

	bool						inited;
	int							score;
	GameSceneStatus				state;
public:
	GameScene(MainMenu& mainMenu);
	void init();
	void reshape(int, int);
	bool handleEvent(const SDL_Event& ev);
	void render();
	void resume();
private:
	void initFonts();
};