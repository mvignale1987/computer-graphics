#include "Cursor.h"

Cursor::Cursor():
	offsetX(0),
	offsetY(0)
{
}

Cursor::Cursor(const std::string& path, int offsetX, int offsetY):
	texture(path, false),
	offsetX(offsetX),
	offsetY(offsetY)
{
}

void Cursor::render(SDL_Window *win) const
{
	if(SDL_GetMouseFocus() != win)
	{
		return;
	}

	int width, height;
	SDL_GetWindowSize(win, &width, &height);
	int mouseX, mouseY;
	SDL_GetMouseState(&mouseX, &mouseY);

	int left = mouseX + offsetX;
	int right = left + texture.width();
	int top = height - mouseY - offsetY;
	int bottom = top - texture.height();

	bool depthEnabled = glIsEnabled(GL_DEPTH_TEST);
	glDisable(GL_DEPTH_TEST);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	{
		glLoadIdentity();
		glOrtho(0, width, 0, height, -1, 1);
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		{
			glLoadIdentity();
			glBindTexture(texture);
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
	if(depthEnabled)
	{
		glEnable(GL_DEPTH_TEST);
	}
}