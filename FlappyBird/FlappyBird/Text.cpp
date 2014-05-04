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

	int left = offsetX;
	int right = left + textTexture.width();
	int top = height - offsetY;
	int bottom = top - textTexture.height();

	if(placement == CENTER)
	{
		int offsetCenterX = width / 2 - textTexture.width() / 2;
		left += offsetCenterX;
		right += offsetCenterX;
		int offsetCenterY = height / 2 - textTexture.height() / 2;
		top -= offsetCenterY;
		bottom -= offsetCenterY;
	}
	
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
				glVertex2i(left, top);
				glTexCoord2f(0, 1);
				glVertex2i(left, bottom);
				glTexCoord2f(1, 1);
				glVertex2i(right, bottom);
				glTexCoord2f(1, 0);
				glVertex2i(right, top);
			}
			glEnd();
		}
		glPopMatrix();
	}
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
}