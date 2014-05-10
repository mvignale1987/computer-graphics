#include "OptionsMenu.h"
#include "Logger.h"
#include <GL/GLU.h>

OptionsMenu::OptionsMenu(MainMenu& mainMenu):
	Scene(mainMenu.app()),
	mainMenu(mainMenu),
	inited(false),
	cursor(NULL),
	backText(NULL),
	interpolatedText(NULL), wireframeText(NULL), texturedText(NULL), useVSyncText(NULL),
	gameSpeedText(NULL), lightColorText(NULL), lightDirectionText(NULL)
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
		addObject(mainMenu.getLogo());
		initFonts();
		initCursor();
	}
	inited = true;
}

void OptionsMenu::reshape(int width, int height)
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

void OptionsMenu::initFonts()
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
	addObject(backText);

	// velocidad de juego
	normalOptions.offsetY = hoverOptions.offsetY = -60;
	gameSpeedText = new TextSwitchHover(
		normalOptions, hoverOptions, menuTick,
		"Velocidad de Juego: Media", "Velocidad de Juego: Alta", "Velocidad de Juego: Baja", NULL
		);
	addObject(gameSpeedText);

	// interpolado
	normalOptions.offsetY = hoverOptions.offsetY +=  40;
	interpolatedText = new TextSwitchHover(
		normalOptions, hoverOptions, menuTick,
		"Renderizado interpolado", "Renderizado liso",  NULL
		);
	addObject(interpolatedText);

	// texturas
	normalOptions.offsetY = hoverOptions.offsetY += 40;
	texturedText = new TextSwitchHover(
		normalOptions, hoverOptions, menuTick,
		"Texturas habilitadas", "Texturas deshabilitadas",  NULL
		);
	addObject(texturedText);

	// texturas
	normalOptions.offsetY = hoverOptions.offsetY += 40;
	useVSyncText = new TextSwitchHover(
		normalOptions, hoverOptions, menuTick,
		"VSync Activado", "VSync Desactivado",  NULL
		);
	addObject(useVSyncText);

	// wireframe
	normalOptions.offsetY = hoverOptions.offsetY += 40;
	wireframeText = new TextSwitchHover(
		normalOptions, hoverOptions, menuTick,
		"Modo alambre desactivado", "Modo alambre activado",  NULL
		);
	addObject(wireframeText);

	// color de luz
	normalOptions.offsetY = hoverOptions.offsetY += 40;
	lightColorText = new TextSwitchHover(
		normalOptions, hoverOptions, menuTick,
		"Luz normal", "Luz roja", "Luz psicodelica", "Luz desactivada",  NULL
		);
	addObject(lightColorText);

	// posición de luz
	normalOptions.offsetY = hoverOptions.offsetY += 40;
	lightDirectionText = new TextSwitchHover(
		normalOptions, hoverOptions, menuTick,
		"Luz de arriba", "Luz de frente", "Luz de adelante", "Luz de atras",  NULL
		);
	addObject(lightDirectionText);

	addObject(mainMenu.getVersionText());
	addObject(mainMenu.getCopyrightText());
}

void OptionsMenu::render()
{
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();	
}

bool OptionsMenu::handleEvent(const SDL_Event& ev)
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

	if(texturedText->wasIndexChanged())
	{
		app().getOptions()->setTexturesEnabled(texturedText->getCurrentIndex() == 0);
	}
	if(useVSyncText->wasIndexChanged())
	{
		app().setVSync(useVSyncText->getCurrentIndex() == 0);
	}

	return true;
}

void OptionsMenu::initCursor()
{
	SDL_ShowCursor(0);
	cursor = new Cursor("cursor.png", -4, -2);
	addObject(cursor);
}