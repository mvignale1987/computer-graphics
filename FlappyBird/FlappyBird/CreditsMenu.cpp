#include "CreditsMenu.h"
#include "Logger.h"
#include <GL/GLU.h>

CreditsMenu::CreditsMenu(MainMenu& mainMenu):
	Scene(mainMenu.app()),
	mainMenu(mainMenu),
	inited(false),
	cursor(NULL),
	backText(NULL),
	integrantes(NULL),
	NB(NULL),
	IA(NULL),
	MV(NULL)
{
}
void CreditsMenu::init()
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glClearColor(Vector3::fromRGB(1, 134, 149));
	glEnable(GL_TEXTURE_2D);

	if(!inited)
	{
		addObject(mainMenu.getClouds());
		addObject(mainMenu.getFloor());
		addObject(mainMenu.getLogo());
		initFonts();
		initCursor();
	}
	inited = true;
}

void CreditsMenu::reshape(int width, int height)
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
void CreditsMenu::initFonts()
{
	TextOptions normalOptions;
	normalOptions.fontPath = "Flappy.ttf";
	normalOptions.fontSize = 32;
	normalOptions.color =  Vector3::one;
	normalOptions.placement = CENTER;
	normalOptions.borderSize = 2;
	normalOptions.borderColor = Vector3::fromRGB(40, 100, 30);

	// volver
	normalOptions.text = "Volver";
	normalOptions.offsetX = 0;
	normalOptions.offsetY = 220;
	Text backTextNormal = Text(normalOptions);	
	// volver -- hover
	TextOptions hoverOptions = normalOptions;
	hoverOptions.fontSize = 36;
	hoverOptions.color =  Vector3::fromRGB(236, 218, 19);
	hoverOptions.text = "Volver";
	Text backTextHover = Text(hoverOptions);
	Mix_Chunk *menuTick = Mix_LoadWAV("MenuTick.wav");
	if(!menuTick) {
		Logger::logSDLError("Mix_LoadWAV");
	}
	backText = new TextHover(backTextNormal, backTextHover, menuTick);

	// version text
	normalOptions.fontSize = 40;
	normalOptions.color =  Vector3::one;
	normalOptions.borderColor = Vector3::fromRGB(40, 100, 30);
	normalOptions.placement = CENTER;
	normalOptions.borderSize = 2;
	normalOptions.offsetX = normalOptions.offsetY = 10;
	normalOptions.text = "Dev Team:";
	integrantes = new Text(normalOptions);
	normalOptions.offsetY += 50;
	normalOptions.text = "Natalia Busiello";
	NB = new Text(normalOptions);
	normalOptions.offsetY += 50;
	normalOptions.text = "Ignacio Avas";
	IA = new Text(normalOptions);
	normalOptions.offsetY += 50;
	normalOptions.text = "Mauricio Vignale";
	MV = new Text(normalOptions);

	addObject(backText);
	addObject(integrantes);
	addObject(NB);
	addObject(IA);
	addObject(MV);

}
void CreditsMenu::initCursor()
{
	SDL_ShowCursor(0);
	cursor = new Cursor("cursor.png", -4, -2);
	addObject(cursor);
}

bool CreditsMenu::handleEvent(const SDL_Event& ev)
{
	// handle if must go back
	if(backText->isClicked(*this))
	{
		app().setScene(&mainMenu);
		return true;
	}
	else if(ev.type == SDL_KEYDOWN &&
			(ev.key.keysym.scancode == SDL_SCANCODE_Q  || ev.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
	)
	{
		app().setScene(&mainMenu);
		return true;
	}
	return true;
	
}
void CreditsMenu::render()
{
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();	
}