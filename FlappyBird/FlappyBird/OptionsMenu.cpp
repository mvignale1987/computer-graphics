#include "OptionsMenu.h"
#include "Logger.h"

OptionsMenu::OptionsMenu(MainMenu& mainMenu):
	Scene(mainMenu.app()),
	mainMenu(mainMenu),
	inited(false)
{
}

void OptionsMenu::init()
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glClearColor(Vector3::fromRGB(1, 134, 149));
	glEnable(GL_TEXTURE_2D);

	if(!inited)
	{
		initFonts();
		initCursor();
	}
	inited = true;
}

void OptionsMenu::initFonts()
{
	TextOptions options;
	options.fontPath = "Flappy.ttf";
	options.fontSize = 42;
	options.color =  Vector3::one;
	options.placement = CENTER;
	options.borderSize = 2;
	options.borderColor = Vector3::fromRGB(40, 100, 30);

	// volver
	options.text = "Volver";
	options.offsetX = options.offsetY = 0;
	Text backTextNormal = Text(options);
	// volver -- hover
	options.fontSize = 46;
	options.color =  Vector3::fromRGB(236, 218, 19);
	options.offsetY = 0;
	options.text = "Volver";
	Text backTextHover = Text(options);

	Mix_Chunk *menuTick = Mix_LoadWAV("MenuTick.wav");
	if(!menuTick) {
		Logger::logSDLError("Mix_LoadWAV");
	}

	backText = new TextHover(backTextNormal, backTextHover, menuTick);
	addObject(backText);
	addObject(mainMenu.getVersionText());
	addObject(mainMenu.getCopyrightText());
}

void OptionsMenu::render()
{
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	if(backText->isClicked(*this))
	{
		app().setScene(&mainMenu);
	}
}

void OptionsMenu::initCursor()
{
	SDL_ShowCursor(0);
	cursor = new Cursor("cursor.png", -4, -2);
	addObject(cursor);
}