#include "Logger.h"
#include "GameScene.h"

GameScene::GameScene(MainMenu& mainMenu):
	Scene(mainMenu.app()),
	mainMenu(mainMenu),
	inited(false),
	flappy(NULL),
	score(NULL),
	state(GAME_SCENE_PLAYING)
{
}
void GameScene::init()
{
	if(!inited)
	{
		addObject(mainMenu.getCamera());
		addObject(mainMenu.getSkybox());
		addObject(mainMenu.getBridge());
		addObject(mainMenu.getFloor());
		flappy = new Flappy(mainMenu.getBridge());
		addObject(flappy);
		dieParticleSystem = new FlappyFeatherParticleSystem(*flappy);
		addObject(dieParticleSystem);
		dieParticleSystem->disable();
		initFonts();
		addObject(mainMenu.getFadeInOut());
		dieEffect = new FadeInOut(0.25f, 0.70f, Vector3::one);
		addObject(dieEffect);
		dieEffect->disable();
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
	normalOptions.offsetY = 10;
	normalOptions.text = "00";
	score = new Text(normalOptions);
	addObject(score);
	normalOptions.color = Vector3::fromRGB(236, 218, 19);
	normalOptions.fontSize = 70;
	normalOptions.placement = CENTER;
	normalOptions.offsetY = -100;
	normalOptions.text = "Preparate";
	getReadyText = new Text(normalOptions);
	addObject(getReadyText);
	normalOptions.text = "Game Over";
	gameOverText = new Text(normalOptions);
	addObject(gameOverText);
	gameOverText->disable();
}


bool GameScene::handleEvent(const SDL_Event& ev)
{

	if(state != GAME_SCENE_FADING_OUT && ev.type == SDL_KEYDOWN)
	{
		switch(ev.key.keysym.scancode){
		case SDL_SCANCODE_P:
		case SDL_SCANCODE_ESCAPE:
			app().setScene(&mainMenu);
			mainMenu.getBridge()->stop();
			mainMenu.getCamera()->disableMove();
			return true;
		case SDL_SCANCODE_Q:
			return false;
		default:
			return true;
		}
	}

	if(state == GAME_SCENE_GAME_OVER &&
		 (ev.type == SDL_MOUSEBUTTONDOWN || ev.type == SDL_KEYDOWN)
	)
	{
		state = GAME_SCENE_FADING_OUT;
		mainMenu.getFadeInOut()->enable();
	}

	return true;
}
void GameScene::render()
{
	switch(state)
	{
	case GAME_SCENE_PLAYING:
		if(flappy->heJumpedFirstTime())
		{
			getReadyText->fadeOut();
		}

		if(flappy->isDead()){
			state = GAME_SCENE_GAME_OVER;
			mainMenu.getBridge()->stop();
			dieEffect->enable();
			dieParticleSystem->enable();
			dieParticleSystem->reset();
			gameOverText->enable();
			gameOverText->fadeIn();
		} 
		break;
	case GAME_SCENE_FADING_OUT:
		if(mainMenu.getFadeInOut()->fadedOut())
		{
			state = GAME_SCENE_PLAYING;
			gameOverText->disable();
			dieParticleSystem->disable();
			getReadyText->enable();
			getReadyText->fadeIn();
			mainMenu.getBridge()->reset();
			mainMenu.getBridge()->resume();
			flappy->respawn();
		}
		break;
	} 
}

void GameScene::resume()
{
	mainMenu.getCamera()->enableMove();
	if(state == GAME_SCENE_PLAYING)
	{
		mainMenu.getBridge()->resume();
	}
}