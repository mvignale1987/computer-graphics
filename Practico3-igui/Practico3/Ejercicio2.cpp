#include <iostream>
#include "Scene.h"
#include "Vector3.h"
#include "Texture.h"
#include "Sprite.h"
#include <GL/GLU.h>
#include <GL/GL.h>
#include <vector>

using namespace std;

class SpriteAnim
{
private:
	float timePerSprite;
	float currentSpriteTime;
	int	currentSprite;
	int spritesList;
	int nSprites;
public:
	SpriteAnim():
		timePerSprite(0),
		currentSpriteTime(0),
		currentSprite(0),
		spritesList(-1),
		nSprites(-1)
	{
	}

	SpriteAnim(int nSprites, int spritesList, float timePerSprite):
		timePerSprite(timePerSprite),
		currentSpriteTime(0),
		currentSprite(0),
		spritesList(spritesList),
		nSprites(nSprites)
	{

	}

	void render(const Scene &s)
	{
		currentSpriteTime += s.getFrameTime();
		if(currentSpriteTime > timePerSprite)
		{
			currentSpriteTime -= timePerSprite;
			currentSprite = (currentSprite + 1) % nSprites;
		}
		glCallList(spritesList + currentSprite);
	}
};

class TextureHorizontalCarousel
{
private:
	GLfloat	speed;
	GLfloat	textureWidth;
	int		displayList;
	GLfloat currentOffset;
public:
	TextureHorizontalCarousel():
		speed(0), textureWidth(0), displayList(-1), currentOffset(0)
	{
	}

	TextureHorizontalCarousel(int displayList, GLfloat textureWidth, GLfloat speed):
		speed(speed), displayList(displayList), textureWidth(textureWidth), currentOffset(0)
	{
	}

	void render(const Scene &s)
	{
		currentOffset += s.getFrameTime() * speed;
		if(currentOffset > textureWidth){
			currentOffset -= textureWidth;
		}

		glMatrixMode(GL_TEXTURE);
		glPushMatrix();
		{
			glTranslate(Vector3::right * currentOffset);
			glCallList(displayList);
		}
		glPopMatrix();
		glMatrixMode(GL_MODELVIEW);
	}
};

class EjScene: public Scene {
private:
	float backgroundSpeed;
	TextureHorizontalCarousel farTextureCarousel;
	TextureHorizontalCarousel midTextureCarousel;
	TextureHorizontalCarousel nearTextureCarousel;
	SpriteAnim frogattoAnim;
public:
	EjScene():
		backgroundSpeed(0.05f)
	{
	}

	void init()
	{
		glEnable(GL_TEXTURE_2D);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glClearColor(1,1,1,1);
		genSpritesList();
		genTextureList();
	}

	void genTextureList()
	{
		Texture bgFar = Texture("forest-dark1.png");
		Texture bgMid = Texture("forest-dark2.png");
		Texture bgNear = Texture("forest-dark5.png");
		int bgFarList = glGenLists(3);
		glNewList (bgFarList, GL_COMPILE);
		{
			glBindTexture( GL_TEXTURE_2D, bgFar.id());

			glBegin(GL_QUADS);
			{
				glTexCoord2f(0, 0);
				glVertex3f(-2,  1, -0.2f);
				glTexCoord2f(0, 2);
				glVertex3f(-2,  -3, -0.2f);
				glTexCoord2f(2, 2);
				glVertex3f( 2,  -3, -0.2f);
				glTexCoord2f(2, 0);
				glVertex3f( 2,  1, -0.2f);
			}
			glEnd();
		}
		glEndList ();
		farTextureCarousel = TextureHorizontalCarousel(bgFarList, 4, backgroundSpeed);
		

		int bgMidList = bgFarList + 1;
		glNewList (bgMidList, GL_COMPILE);
		{
			glBindTexture( GL_TEXTURE_2D, bgMid.id());
			glBegin(GL_QUADS);
			{
				glTexCoord2f(0, 0);
				glVertex3f(-2,  1.0f, -0.1f);
				glTexCoord2f(0, 2);
				glVertex3f(-2,  -3, -0.1f);
				glTexCoord2f(2, 2);
				glVertex3f( 2,  -3, -0.1f);
				glTexCoord2f(2, 0);
				glVertex3f( 2,  1, -0.1f);
			}
			glEnd();
		}
		glEndList ();
		midTextureCarousel = TextureHorizontalCarousel(bgMidList, 4, backgroundSpeed * 2);

		int bgNearList = bgFarList + 2;
		glNewList (bgNearList, GL_COMPILE);
		{
			glBindTexture( GL_TEXTURE_2D, bgNear.id());
			glBegin(GL_QUADS);
			{
				glTexCoord2f(0, 0);
				glVertex3f(-2,  -0.125f, 0.1f);
				glTexCoord2f(0, 2);
				glVertex3f(-2,  -2.125f, 0.1f);
				glTexCoord2f(2, 2);
				glVertex3f( 2,  -2.125f, 0.1f);
				glTexCoord2f(2, 0);
				glVertex3f( 2,  -0.125f, 0.1f);
			}
			glEnd();
		}
		glEndList ();
		nearTextureCarousel = TextureHorizontalCarousel(bgNearList, 4, backgroundSpeed * 4);
	}

	void genSpritesList()
	{
		vector<Sprite> sprites = Sprite::loadFromImageAndXML("frogatto-spritesheet1.png", "sprites.xml", 1);
		int nSprites = sprites.size();
		int spritesList = glGenLists(nSprites);

		for(int i = 0; i < nSprites; ++i){
			glNewList (spritesList + i, GL_COMPILE);
			{
				Sprite sprite = sprites.at(i);
				glBindTexture( GL_TEXTURE_2D, sprite.id());

				glPushMatrix();
				{
					glLoadIdentity();
					glBegin(GL_QUADS);
					{
						sprite.glTexCoordTopLeft();
						glVertex2f(-0.2f, -0.2f);
						sprite.glTexCoordBottomLeft();
						glVertex2f(-0.2f, -0.6f);
						sprite.glTexCoordBottomRight();
						glVertex2f( 0.2f, -0.6f);
						sprite.glTexCoordTopRight();
						glVertex2f( 0.2f, -0.2f);
					}
					glEnd();
				}
				glPopMatrix();
			}
			glEndList ();
		}
		frogattoAnim = SpriteAnim(nSprites, spritesList, 0.1f);
	}

	void reshape(int w, int h)
	{
		glViewport(0, 0, w, h);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		if (w <= h) 
		  gluOrtho2D (
			-1.0, 1.0,
			-(GLfloat) h/(GLfloat) w, (GLfloat) h/(GLfloat) w
		);
	   else 
		  gluOrtho2D (
		  -(GLfloat) w/(GLfloat) h, (GLfloat) w/(GLfloat) h,
		  -1, 1
		  ); 
	}

	void render()
	{
		glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glMatrixMode(GL_TEXTURE);
		farTextureCarousel.render(*this);
		midTextureCarousel.render(*this);
		frogattoAnim.render(*this);
		nearTextureCarousel.render(*this);
	}

	void clean()
	{
	}
};


int main(int argc, char **argv)
{
	return EjScene().run();
}