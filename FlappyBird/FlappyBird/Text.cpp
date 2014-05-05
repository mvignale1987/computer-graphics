#include "Text.h"
#include <GL/GLU.h>

Text::Text():
	offsetX(0),
	offsetY(0),
	placement(TOP_LEFT)
{
}

Text::Text(const std::string& text, TTF_Font *font, Vector3 color, Placement placement, int offsetX, int offsetY):
	offsetX(offsetX),
	offsetY(offsetY),
	placement(placement)
{
	SDL_Surface *textSurface = TTF_RenderText_Blended(font, text.c_str(), color.toSDLColor());
	textTexture = Texture(textSurface, false);
	SDL_FreeSurface(textSurface);
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