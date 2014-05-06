#pragma once

#include <SDL.h>
#include <SDL_ttf.h>
#include <string>
#include "Texture.h"
#include "Vector3.h"
#include "Rect.h"

enum Placement { CENTER, TOP_LEFT }; // Text placement in screen

class TextOptions;

class Text
{
private:
	Texture textTexture;
	int offsetX, offsetY;
	Placement placement;
public:
	Text();

	Text(const TextOptions& options);

	bool mouseHover(SDL_Window *win) const; // true if the mouse is in the font bounding rect
	bool mouseClick(SDL_Window *win) const; // true if the mouse if clicked  in the font bounding rect
	void render(SDL_Window *win) const; // renders the text within window coordinates
private:
	Rect getBoundingRect(SDL_Window *win) const;
};

class TextOptions // options for constructing a text object
{
public:
	std::string text;
	std::string fontPath;
	int			fontSize;
	Vector3		color;
	Vector3		borderColor;
	Placement	placement;
	int			offsetX;
	int			offsetY;
	int			borderSize;

	TextOptions();
};