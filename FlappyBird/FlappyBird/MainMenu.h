#pragma once
#include "Scene.h"
#include "Texture.h"
#include "TextHover.h"
#include "Cursor.h"
#include "Floor.h"
#include "MainMenuLogo.h"
#include <SDL_ttf.h>
#include <SDL_mixer.h>

class OptionsMenu;

class MainMenu: public Scene
{
private:
	OptionsMenu		*optionsMenu;
	MainMenuLogo	*logo;
	TextHover		*playText, *optionsText, *quitText;
	Text			*versionText, *copyrightText;
	Floor			*floor;
	Cursor			*cursor;
	bool			quitClicked;
	Mix_Music		*music;
public:
	MainMenu(App &parent);
	void init();
	void reshape(int w, int h);
	bool handleEvent(const SDL_Event& ev);
	void render();

	Floor * getFloor() const;
	MainMenuLogo *getLogo() const;
	Text *getVersionText() const;
	Text *getCopyrightText() const;
private:
	void initFonts();
	void initMusic();
	void initCursor();
};