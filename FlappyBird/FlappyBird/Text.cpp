#include "Text.h"
#include "SceneError.h"
#include <GL/GLU.h>

Text::Text():
	offsetX(0),
	offsetY(0),
	placement(TOP_LEFT)
{
}

Text::Text(const TextOptions& options):
	offsetX(options.offsetX),
	offsetY(options.offsetY),
	placement(options.placement)
{
	TTF_Font *font = TTF_OpenFont(options.fontPath.c_str(), options.fontSize);
	if (font == NULL){
		throw SceneError::fromSDLError("Couldn't load font: TTF_OpenFont");
    }
	const char *text = options.text.c_str();

	SDL_Surface *textSurface = TTF_RenderText_Blended(font, text, options.color.toSDLColor());
	if(options.borderSize == 0)
	{
		textTexture = Texture(textSurface, false);
	} else {
		TTF_SetFontOutline(font, options.borderSize);
		SDL_Surface *borderSurface = TTF_RenderText_Blended(font, text, options.borderColor.toSDLColor());

		SDL_Rect destinationRect = { options.borderSize, options.borderSize, textSurface->w, textSurface->h };
		if(SDL_BlitSurface(textSurface, NULL, borderSurface, &destinationRect) < 0){
			throw SceneError::fromSDLError("Couldn't load font: SDL_BlitSurface");
		}
		textTexture = Texture(borderSurface, false);
		SDL_FreeSurface(borderSurface);
	} 

	SDL_FreeSurface(textSurface);
	TTF_CloseFont(font);
}

void Text::render(SDL_Window *win) const
{
	int width, height;
	SDL_GetWindowSize(win, &width, &height);

	Rect rect = getBoundingRect(win);
	
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	{
		glLoadIdentity();
		glOrtho(0, width, 0, height, -1, 1);
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		{
			glLoadIdentity();
			glBindTexture(textTexture);
			glBegin(GL_QUADS);
			{
				glTexCoord2f(0, 0);
				glVertex2f(rect.left(), height - rect.top());
				glTexCoord2f(0, 1);
				glVertex2f(rect.left(), height - rect.bottom());
				glTexCoord2f(1, 1);
				glVertex2f(rect.right(), height - rect.bottom());
				glTexCoord2f(1, 0);
				glVertex2f(rect.right(), height - rect.top());
			}
			glEnd();
		}
		glPopMatrix();
	}
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
}

bool Text::mouseHover(SDL_Window *win) const 
{
	if(SDL_GetMouseFocus() != win)
	{
		return false;
	}
	int mouseX, mouseY;
	SDL_GetMouseState(&mouseX, &mouseY);
	return getBoundingRect(win).contains(mouseX, mouseY);
}

bool Text::mouseClick(SDL_Window *win) const 
{
	if(SDL_GetMouseFocus() != win)
	{
		return false;
	}
	int mouseX, mouseY;
	int mouseState = SDL_GetMouseState(&mouseX, &mouseY);
	if((mouseState & (SDL_BUTTON(1) | SDL_BUTTON(2))) == 0)
		return false;
	return getBoundingRect(win).contains(mouseX, mouseY);
}

Rect Text::getBoundingRect(SDL_Window *win) const
{
	int width, height;
	SDL_GetWindowSize(win, &width, &height);

	int left = offsetX;
	int right = left + textTexture.width();
	int top = offsetY;
	int bottom = top + textTexture.height();

	if(placement == CENTER)
	{
		int offsetCenterX = width / 2 - textTexture.width() / 2;
		left += offsetCenterX;
		right += offsetCenterX;
		int offsetCenterY = height / 2 - textTexture.height() / 2;
		top += offsetCenterY;
		bottom += offsetCenterY;
	}

	return Rect(top, left, bottom, right);
}


TextOptions::TextOptions():
	text(""),
	fontPath(""),
	fontSize(-1),
	color(Vector3::one),
	borderColor(Vector3::zero),
	placement(TOP_LEFT),
	offsetX(0),
	offsetY(0),
	borderSize(0)
{
}