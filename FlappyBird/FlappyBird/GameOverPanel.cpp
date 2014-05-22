#include "GameOverPanel.h"
#include "Vector3.h"
#include "SceneError.h"
#include <sstream>
#include <SDL_image.h>

const float GameOverPanel::coolDownTime = 1.0f;
const int GameOverPanel::translationYLength = 100;
const float GameOverPanel::animTotalTime = 0.5f + GameOverPanel::coolDownTime;


GameOverPanel::GameOverPanel():
	texture(),
	animTime(0),
	highScore(0),
	score(0)
{
	scoreFont = TTF_OpenFont("Flappy.ttf", 25);
	if (scoreFont == NULL){
			throw SceneError::fromSDLError("Couldn't load font: TTF_OpenFont");
	}
}

void GameOverPanel::render(Scene& parent)
{
	SDL_Window * win = parent.app().getWindow();
	int width, height;
	SDL_GetWindowSize(win, &width, &height);

	Rect rect = getBoundingRect(win);
	
	glPushAttrib(GL_ENABLE_BIT);
	{
		glMatrixMode(GL_PROJECTION);
		glDisable(GL_LIGHTING);
		glPushMatrix();
		{
			glLoadIdentity();
			glOrtho(0, width, 0, height, -1, 1);
			glMatrixMode(GL_MODELVIEW);
			glPushMatrix();
			{
				glLoadIdentity();
				glBindTexture(texture);
				
				float alpha = 0;
				float translateY = 0;
				if(animTime > coolDownTime)
				{
					float diff = (animTime - coolDownTime) / (animTotalTime - coolDownTime);
					if(diff > 1)
						diff = 1;
					alpha = diff;
					translateY = translationYLength * (1 - diff) * (1 - diff);
				}
				glTranslate(Vector3::down * translateY);
				glColor(Vector3::one, alpha); 
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
	glPopAttrib();

	if(animTime < animTotalTime)
	{
		animTime += parent.app().getFrameTime();
	}
}

void GameOverPanel::enable(int currentScore)
{
	SceneObject::enable();
	animTime = 0;
	score = currentScore;
	if(highScore > currentScore)
	{
		highScore = currentScore;
	}

	stringstream ss;
	ss << score;

	// copy texts to texture
	SDL_Surface *panelSurface = IMG_Load("panel.png");
	if(panelSurface == NULL)
	{
		throw exception("Couldn't load texture panel.png");
	}
	SDL_Surface *scoreSurface = Text::getSurface(scoreFont, ss.str(), Vector3::one, 2, Vector3::zero);

	SDL_Rect dstRect = {
		panelSurface->w / 2 + 170,
		panelSurface->h / 2 + 10,
		scoreSurface->w,
		scoreSurface->h
	};
	if(SDL_BlitSurface(scoreSurface, NULL, panelSurface, &dstRect) < 0)
	{
		throw SceneError::fromSDLError("Couldn't copy surface");
	}

	glDeleteTextures(texture);
	texture = Texture(panelSurface);
}


Rect GameOverPanel::getBoundingRect(SDL_Window *win)
{
	return Rect::createBoundingRect(win, CENTER, 0, 50, texture.width(), texture.height());
}
