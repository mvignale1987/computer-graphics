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
	TTF_Font *font = TTF_OpenFont("Flappy.ttf", 42);
	if (font == NULL){
		throw SceneError::fromSDLError("Couldn't load font: TTF_OpenFont");
    }
	Vector3 white = Vector3::fromRGB(244,249,245);
	playText = Text("Jugar", font, white, CENTER, 0, 0);
	optionsText = Text("Opciones", font, white, CENTER, 0, 50);
	quitText = Text("Salir", font, white, CENTER, 0, 100);
	TTF_CloseFont(font);

	TTF_Font *fontHover = TTF_OpenFont("Flappy.ttf", 46);
	if (fontHover == NULL){
		throw SceneError::fromSDLError("Couldn't load font: TTF_OpenFont");
    }
	Vector3 yellow = Vector3::fromRGB(236, 218, 19);
	playTextHover = Text("Jugar", fontHover, yellow, CENTER, 0, 0);
	optionsTextHover = Text("Opciones", fontHover, yellow, CENTER, 0, 50);
	quitTextHover = Text("Salir", fontHover, yellow, CENTER, 0, 100);
	TTF_CloseFont(fontHover);
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