#pragma once

#include <SDL_opengl.h>
#include <string>

using namespace std;

class Texture
{
private:
	GLuint textureId;
	GLuint w;
	GLuint h;
public:
	Texture(); // dummy constructor
	Texture(int id, GLuint width, GLuint height); // creates a texture from an OpenGL texture id, and dimensions
	Texture(string path); // load a texture from a path

	GLuint id() const; // texture OpenGL id
	GLuint width() const; // texture width
	GLuint height() const; // texture height
	void glTexCoord(int x, int y) const; // shortcut for glTexCoord2f(x/(GLfloat) width() , y/(GLfloat) height() );
};

void glBindTexture(const Texture& t);