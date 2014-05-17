#include "Logger.h"
#include "GameScene.h"

GameScene::GameScene(MainMenu& mainMenu):
	Scene(mainMenu.app()),
	mainMenu(mainMenu),
	inited(false),
	flappy(NULL),
	score(NULL)
{
}
void GameScene::init()
{
	flappy = new Flappy(mainMenu.getBridge());
	if(!inited)
	{
		addObject(mainMenu.getCamera());
		addObject(mainMenu.getSkybox());
		addObject(mainMenu.getBridge());
		addObject(mainMenu.getFloor());
		addObject(flappy);
		initFonts();
	}
	inited = true;
}

void GameScene::reshape(int width, int height)
{
	mainMenu.getCamera()->reshape(width, height);
}
void GameScene::initFonts()
{
	TextOptions normalOptions;
	normalOptions.fontPath = "Flappy.ttf";
	normalOptions.fontSize = 72;
	normalOptions.color =  Vector3::one;
	normalOptions.placement = TOP_CENTER;
	normalOptions.borderSize = 3;
	normalOptions.borderColor = Vector3::fromRGB(40, 100, 30);
	normalOptions.offsetX = 10;
	normalOptions.offsetY = 10;
	normalOptions.text = "00";
	score = new Text(normalOptions);
	addObject(score);
}


bool GameScene::handleEvent(const SDL_Event& ev)
{
	 if(ev.type == SDL_KEYDOWN &&
			(ev.key.keysym.scancode == SDL_SCANCODE_Q  || ev.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
	)
	{
		app().setScene(&mainMenu);
	}
	return true;
}
void GameScene::render()
{
}