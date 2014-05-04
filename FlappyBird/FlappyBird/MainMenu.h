#pragma once
#include "Scene.h"
#include "Texture.h"

class MainMenu: public Scene
{
private:
	Texture logo;
	GLfloat logoAnimTime;
protected:
	void init();
	void reshape(int w, int h);
	void render();
	std::string windowTitle();
	std::string appIconPath();
private:
	void renderLogo();
};