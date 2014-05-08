#include "TextHover.h"
#include "Logger.h"

TextHover::TextHover():
	hoverSound(NULL)
{
}

TextHover::TextHover(const Text& normal, const Text &hover, Mix_Chunk *hoverSound):
	normal(normal),
	hover(hover),
	hoverSound(hoverSound)
{
}

void TextHover::render(Scene& parent)
{
	SDL_Window *win = parent.app().getWindow();

	bool wasHover = isHover;
	isHover = normal.mouseHover(win);
	(isHover ? hover : normal).render(win);
	if(hoverSound && isHover && !wasHover) {
		if(Mix_PlayChannel(-1, hoverSound, 0) == -1) {
			Logger::logSDLError("Unable to play WAV file");
		}
	}
}

bool TextHover::isClicked(Scene &parent) const
{
	return normal.mouseClick(parent.app().getWindow());
}