#include "MainMenu.h"
#include "Vector3.h"
#include "SceneError.h"
#include <GL/GLU.h>

MainMenu::MainMenu():
	music(NULL),
	menuTick(NULL),
	playTextIsHover(false), optionsTextIsHover(false), quitTextIsHover(false),
	quitClicked(false)
{
}

void MainMenu::init()
{
	logoAnimTime = 0;
	logo = Texture("logo.png");
	
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glClearColor(Vector3::fromRGB(1, 134, 149));
	glEnable(GL_TEXTURE_2D);

	// init cursor
	SDL_ShowCursor(0);
	cursor = Cursor("cursor.png", -4, -2);
	
	initFonts();
	initMusic();
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

bool MainMenu::handleEvent(SDL_Event)
{
	return !quitClicked;
}

void MainMenu::render()
{
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	float angle = sinf(logoAnimTime * 2 * (GLfloat) M_PI) * 6.0f;
	float offsetDistance = sinf(logoAnimTime * 3 * (GLfloat) M_PI) * 0.15f;

	glPushMatrix();
	{
		glTranslate(Vector3::up);
		glTranslate(Vector3::backward * (4 + offsetDistance));
		glRotate(angle, Vector3::forward);
		logo.render();
	}
	glPopMatrix();

	renderTextsAndSounds();

	cursor.render(getWindow());

	logoAnimTime += getFrameTime() * 0.05f;
	if(logoAnimTime > 6)
		logoAnimTime -= 6;
}

std::string MainMenu::windowTitle()
{
	return "FlappyBird";
}

std::string MainMenu::appIconPath()
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
	playText = Text(options);
	// opciones
	options.text = "Opciones";
	options.offsetY = 50;
	optionsText = Text(options);
	// salir
	options.text = "Salir";
	options.offsetY = 100;
	quitText = Text(options);

	// jugar -- hover
	options.fontSize = 46;
	options.color =  Vector3::fromRGB(236, 218, 19);
	options.offsetY = 0;
	options.text = "Jugar";
	playTextHover = Text(options);
	// opciones -- hover
	options.text = "Opciones";
	options.offsetY = 50;
	optionsTextHover = Text(options);
	// salir -- hover
	options.text = "Salir";
	options.offsetY = 100;
	quitTextHover = Text(options);
}

void MainMenu::initMusic()
{
	int flags = MIX_INIT_MP3;
	int initted = Mix_Init(flags);
	if ((initted & flags) != flags) {
		logSDLError("Mix_Init: Failed to init required support");
		return;
	}
	if(Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 1024) != 0) {
		logSDLError("Mix_OpenAudio: Unable to initialize audio");
		return;
	}
	music = Mix_LoadMUS("MenuMusic.mp3");
	if(!music) {
		logSDLError("Mix_LoadMUS error");
		return;
	}
	if(Mix_PlayMusic(music, -1) == -1) {
		logSDLError("Mix_PlayMusic");
		return;
	}
	menuTick = Mix_LoadWAV("MenuTick.wav");
	if(!menuTick) {
		logSDLError("Mix_LoadWAV");
		return;
	}
}

void MainMenu::renderTextsAndSounds()
{
	SDL_Window *win = getWindow();

	bool playTextWasHover = playTextIsHover;
	bool optionsTextWasHover = optionsTextIsHover;
	bool quitTextWasHover = quitTextIsHover;

	playTextIsHover = playTextHover.mouseHover(win);
	optionsTextIsHover = optionsText.mouseHover(win);
	quitTextIsHover = quitText.mouseHover(win);

	(playTextIsHover ? playTextHover : playText).render(win);
	(optionsTextIsHover ? optionsTextHover : optionsText).render(win);
	(quitTextIsHover ? quitTextHover : quitText).render(win);

	if(playTextIsHover && !playTextWasHover ||
		optionsTextIsHover && !optionsTextWasHover ||
		quitTextIsHover && !quitTextWasHover) {
			if(Mix_PlayChannel(-1, menuTick, 0) == -1) {
				logSDLError("Unable to play WAV file");
			}
	}

	quitClicked = quitClicked || quitText.mouseClick(win);
}