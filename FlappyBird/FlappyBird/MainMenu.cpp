#include "MainMenu.h"
#include "Vector3.h"
#include "SceneError.h"
#include "SceneObject.h"
#include "Logger.h"
#include <GL/GLU.h>

MainMenu::MainMenu(App& parent):
	Scene(parent),
	music(NULL),
	quitClicked(false)
{
}

void MainMenu::init()
{	
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glClearColor(Vector3::fromRGB(1, 134, 149));
	glEnable(GL_TEXTURE_2D);

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
	gluPerspective(45.0f,(GLfloat)width/(GLfloat)height, 0.1f, 100.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

bool MainMenu::handleEvent(const SDL_Event&)
{
	return !quitClicked;
}

void MainMenu::render()
{
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	quitClicked = quitClicked || quitText->isClicked(*this);
}

std::string MainMenu::windowTitle() const
{
	return "FlappyBird";
}

std::string MainMenu::appIconPath() const
{
	return "icon.png";
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