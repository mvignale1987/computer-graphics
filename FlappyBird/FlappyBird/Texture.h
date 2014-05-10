#pragma once

#include <SDL.h>
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
	Texture(string path, bool lineal = true, bool repeat = false); // load a texture from a path (with lineal filtering)
	Texture(SDL_Surface *surface, bool lineal = true, bool repeat = false); // load a texture from a surface (with lineal filtering)

	GLuint id() const; // texture OpenGL id
	GLuint width() const; // texture width
	GLuint height() const; // texture height
	void glTexCoord(int x, int y) const; // shortcut for glTexCoord2f(x/(GLfloat) width() , y/(GLfloat) height() );
	
	void render() const; // renders the texture entirely
private:
	void initFromSurface(SDL_Surface *surface, bool lineal, bool repeat);
};

void glBindTexture(const Texture& t);
void glDeleteTextures(const Texture& t);