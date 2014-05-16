#pragma once
#include "Scene.h"
#include "Texture.h"
#include "TextHover.h"
#include "Cursor.h"
#include "Floor.h"
#include "Skybox.h"
#include "MainMenuLogo.h"
#include "Camera.h"
#include "Bridge.h"

#include <SDL_ttf.h>
#include <SDL_mixer.h>

class OptionsMenu;
class CreditsMenu;
class GameScene;

class MainMenu: public Scene
{
private:
	OptionsMenu		*optionsMenu;
	CreditsMenu		*creditsMenu;
	GameScene		*gameScene;
	MainMenuLogo	*logo;
	TextHover		*playText, *optionsText, *quitText,*copyrightText;
	Text			*versionText;
	Floor			*floor;
	Skybox			*skybox;
	Camera			*camera;
	Bridge			*bridge;
	Cursor			*cursor;
	bool			quitClicked;
	Mix_Music		*music;
public:
	MainMenu(App &parent);
	void init();
	void reshape(int w, int h);
	bool handleEvent(const SDL_Event& ev);
	void render();

	Skybox *getSkybox() const;
	Floor *getFloor() const;
	MainMenuLogo *getLogo() const;
	Text *getVersionText() const;
	Camera *getCamera() const;
	Bridge *getBridge() const;
//	Text *getCopyrightText() const;
private:
	void initFonts();
	void initMusic();
	void initCursor();
};