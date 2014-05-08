#pragma once
#include "Scene.h"
#include "Texture.h"
#include "TextHover.h"
#include "Cursor.h"
#include "MainMenuLogo.h"
#include <SDL_ttf.h>
#include <SDL_mixer.h>

class MainMenu: public Scene
{
private:
	MainMenuLogo	*logo;
	TextHover		*playText, *optionsText, *quitText;
	Cursor			*cursor;
	bool			quitClicked;
	Mix_Music		*music;
public:
	MainMenu(App &parent);
protected:
	void init();
	void reshape(int w, int h);
	bool handleEvent(const SDL_Event& ev);
	void render();
	std::string windowTitle() const;
	std::string appIconPath() const;
private:
	void initFonts();
	void initMusic();
	void initCursor();
};