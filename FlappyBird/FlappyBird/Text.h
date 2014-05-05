#pragma once

#include <SDL.h>
#include <SDL_ttf.h>
#include <string>
#include "Texture.h"
#include "Vector3.h"
#include "Rect.h"

enum Placement { CENTER, TOP_LEFT }; // Text placement in screen

class Text
{
private:
	Texture textTexture;
	int offsetX, offsetY;
	Placement placement;
public:
	Text();

	Text(const std::string& text, TTF_Font *font, Vector3 color = Vector3::one,
		Placement placement = TOP_LEFT, int offsetX = 0, int offsetY = 0);

	bool mouseHover(SDL_Window *win) const; // true if the mouse is in the font bounding rect
	void render(SDL_Window *win) const; // renders the text within window coordinates
private:
	Rect getBoundingRect(SDL_Window *win) const;
};