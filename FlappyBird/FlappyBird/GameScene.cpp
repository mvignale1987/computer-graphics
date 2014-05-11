#include "Logger.h"
#include "GameScene.h"
#include <GL/GLU.h>

GameScene::GameScene(MainMenu& mainMenu):
	Scene(mainMenu.app()),
	mainMenu(mainMenu),
	inited(false),
	integrantes(NULL),
	NB(NULL),
	IA(NULL),
	MV(NULL),
	pajarito(NULL),
	tubo1(NULL),
	tubo2(NULL)
{
}
void GameScene::init()
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glClearColor(Vector3::fromRGB(1, 134, 149));
	glEnable(GL_TEXTURE_2D);
	pajarito = new Flappy();
	tubo1 =	new Pipe(false);
	tubo2 = new Pipe(true);
	if(!inited)
	{
		addObject(tubo1);
		addObject(tubo2);
		addObject(pajarito);
		addObject(mainMenu.getClouds());
		addObject(mainMenu.getFloor());
		initFonts();
	}
	inited = true;
}

void GameScene::reshape(int width, int height)
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
void GameScene::initFonts()
{
	TextOptions normalOptions;
	normalOptions.fontPath = "Flappy.ttf";
	normalOptions.fontSize = 20;
	normalOptions.color =  Vector3::one;
	normalOptions.placement = TOP_LEFT;
	normalOptions.borderSize = 1;
	normalOptions.borderColor = Vector3::fromRGB(40, 100, 30);


}


bool GameScene::handleEvent(const SDL_Event& ev)
{
	//// handle if must go back
	//if(backText->isClicked(*this))
	//{
	//	app().setScene(&mainMenu);
	//	return true;
	//}
	//else if(ev.type == SDL_KEYDOWN &&
	//		(ev.key.keysym.scancode == SDL_SCANCODE_Q  || ev.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
	//)
	//{
	//	app().setScene(&mainMenu);
	//	return true;
	//}
	return true;
	
}
void GameScene::render()
{
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();	
}