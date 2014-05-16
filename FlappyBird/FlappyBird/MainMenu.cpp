#include "MainMenu.h"
#include "Vector3.h"
#include "SceneError.h"
#include "SceneObject.h"
#include "Logger.h"
#include "OptionsMenu.h"
#include "GameScene.h"
#include "CreditsMenu.h"
#include <GL/GLU.h>

MainMenu::MainMenu(App& parent):
	Scene(parent),
	music(NULL),
	quitClicked(false),
	playText(NULL), optionsText(NULL), quitText(NULL),
	versionText(NULL), copyrightText(NULL),
	floor(NULL), skybox(NULL)
{
	optionsMenu = new OptionsMenu(*this);
	creditsMenu = new CreditsMenu(*this);
	gameScene	= new GameScene(*this);
}

void MainMenu::init()
{	
	glEnable(GL_DEPTH_TEST);
	// blend
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glClearColor(Vector3::fromRGB(86, 157, 223));
	glEnable(GL_TEXTURE_2D);
	glShadeModel(GL_SMOOTH);		 // Enables Smooth Shading
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculation

	// fog
	glEnable (GL_FOG);
	glHint(GL_FOG_HINT, GL_NICEST);
	glFogi (GL_FOG_MODE, GL_EXP2);
	glFogf (GL_FOG_DENSITY, 0.001f);
	glFog (GL_FOG_COLOR, Vector3::fromRGB(86, 157, 223));

	// lighting
	GLfloat LightAmbient[4] = { 0.5f, 0.5f, 0.5f, 1.0f };
	GLfloat LightDiffuse[4] = { 1, 1, 1, 1};
	GLfloat LightPosition[4] = { 0, 0, 15, 1 };
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);    // Uses default lighting parameters
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
	glEnable(GL_NORMALIZE);
	glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);
	glLightfv(GL_LIGHT1, GL_POSITION, LightPosition);
	glEnable(GL_LIGHT1);

	camera = new Camera(0, 1.5f, 270.0f, Vector3::up * 120.0f);
	addObject(camera);
	skybox = new Skybox(camera);
	addObject(skybox);
	bridge = new Bridge(camera);
	addObject(bridge);
	floor = new Floor(camera);
	addObject(floor);
	logo = new MainMenuLogo("logo.png");
	addObject(logo);
	initMusic();
	initFonts();
	initCursor();
}

void MainMenu::reshape(int width, int height)
{
	if (height==0)
	{
		height=1;
	}
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f,(GLfloat)width/(GLfloat)height, 0.1f, 2000.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

bool MainMenu::handleEvent(const SDL_Event& ev)
{
	if(ev.type == SDL_KEYDOWN &&
			(ev.key.keysym.scancode == SDL_SCANCODE_Q  || ev.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
	)
	{
		return false;
	}

	if(ev.type == SDL_MOUSEBUTTONDOWN && optionsText->isClicked(*this))
	{
		optionsMenu->init();
		app().setScene(optionsMenu);
	}
	else if(ev.type == SDL_MOUSEBUTTONDOWN && copyrightText->isClicked(*this))
	{
		creditsMenu->init();
		app().setScene(creditsMenu);
	}
	else if(ev.type == SDL_MOUSEBUTTONDOWN && copyrightText->isClicked(*this)){
		gameScene->init();
		app().setScene(gameScene);
	
	}


	return !quitClicked;
}

void MainMenu::render()
{
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glDisable(GL_CULL_FACE);
	glLoadIdentity();
	quitClicked = quitClicked || quitText->isClicked(*this);


	if(optionsText->isClicked(*this))
	{
		optionsMenu->init();
		app().setScene(optionsMenu);
	}else if(copyrightText->isClicked(*this)){
		
		creditsMenu->init();
		app().setScene(creditsMenu);
	} 
	else if(playText->isClicked(*this)){
		gameScene->init();
		app().setScene(gameScene);
	}

}

void MainMenu::initFonts()
{
	TextOptions options;
	options.fontPath = "Flappy.ttf";
	options.fontSize = 42;
	options.color =  Vector3::one;
	options.placement = CENTER;
	options.borderSize = 2;
	options.borderColor = Vector3::fromRGB(40, 100, 30);

	// jugar
	options.text = "Jugar";
	options.offsetX = options.offsetY = 0;
	Text playTextNormal = Text(options);
	// opciones
	options.text = "Opciones";
	options.offsetY = 50;
	Text optionsTextNormal = Text(options);
	// salir
	options.text = "Salir";
	options.offsetY = 100;
	Text quitTextNormal = Text(options);
	

	options.placement = CENTER;
	// jugar -- hover
	options.fontSize = 46;
	options.color =  Vector3::fromRGB(236, 218, 19);
	options.offsetY = 0;
	options.text = "Jugar";
	Text playTextHover = Text(options);
	// opciones -- hover
	options.text = "Opciones";
	options.offsetY = 50;
	Text optionsTextHover = Text(options);
	// salir -- hover
	options.text = "Salir";
	options.offsetY = 100;
	Text quitTextHover = Text(options);

	

	Mix_Chunk *menuTick = Mix_LoadWAV("MenuTick.wav");
	if(!menuTick) {
		Logger::logSDLError("Mix_LoadWAV");
	}

	playText = new TextHover(playTextNormal, playTextHover, menuTick);
	optionsText = new TextHover(optionsTextNormal, optionsTextHover, menuTick);
	quitText = new TextHover(quitTextNormal, quitTextHover, menuTick);
	

	addObject(playText);
	addObject(optionsText);
	addObject(quitText);

	// version text
	options.fontSize = 20;
	options.color =  Vector3::one;
	options.placement = BOTTOM_LEFT;
	options.borderSize = 1;
	options.offsetX = options.offsetY = 10;
	options.text = "v0.0.1";
	versionText = new Text(options);

	// copyright text
	options.placement = BOTTOM_RIGHT;
	options.text = "2014 CompGraf";
	Text copyrightTextNormal =  Text(options);

	// copyright text -- hover
	options.fontSize = 22;
	options.color =  Vector3::fromRGB(236, 218, 19);
	options.text = "2014 CompGraf";
	Text copyrightTextHover = Text(options);

	copyrightText = new TextHover(copyrightTextNormal, copyrightTextHover, menuTick);

	addObject(versionText);
	addObject(copyrightText);
}

void MainMenu::initMusic()
{
	int flags = MIX_INIT_MP3;
	int initted = Mix_Init(flags);
	if ((initted & flags) != flags) {
		Logger::logSDLError("Mix_Init: Failed to init required support");
		return;
	}
	if(Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 1024) != 0) {
		Logger::logSDLError("Mix_OpenAudio: Unable to initialize audio");
		return;
	}
	music = Mix_LoadMUS("MenuMusic.mp3");
	if(!music) {
		Logger::logSDLError("Mix_LoadMUS error");
		return;
	}
	if(Mix_PlayMusic(music, -1) == -1) {
		Logger::logSDLError("Mix_PlayMusic");
		return;
	}
}

void MainMenu::initCursor()
{
	SDL_ShowCursor(0);
	cursor = new Cursor("cursor.png", -4, -2);
	addObject(cursor);
}


Floor *MainMenu::getFloor() const
{
	return floor;
}

MainMenuLogo *MainMenu::getLogo() const 
{
	return logo;
}

Camera *MainMenu::getCamera() const
{
	return camera;
}

Bridge *MainMenu::getBridge() const 
{
	return bridge;
}

Skybox *MainMenu::getSkybox() const
{
	return skybox;
}

Text *MainMenu::getVersionText() const
{
	return versionText;
}