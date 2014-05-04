#include "Texture.h"
#include <SDL_image.h>


Texture::Texture():
	textureId(-1), w(0), h(0)
{
}

Texture::Texture(int id, GLuint width, GLuint height):
	textureId(id), w(width), h(height)
{
}

Texture::Texture(string path)
{
	SDL_Surface *surface;	// This surface will tell us the details of the image
	GLenum texture_format;
	

	if(!(surface = IMG_Load(path.c_str())))
	{
		throw exception(("Couldn't load sprite " + path).c_str());
	}

 
	// get the number of channels in the SDL surface
	GLint nOfColors = surface->format->BytesPerPixel;
	if (nOfColors == 4)     // contains an alpha channel
	{
		if (surface->format->Rmask == 0x000000ff)
				texture_format = GL_RGBA;
		else
				texture_format = GL_BGRA;
	} else if (nOfColors == 3)     // no alpha channel
	{
		if (surface->format->Rmask == 0x000000ff)
				texture_format = GL_RGB;
		else
				texture_format = GL_BGR;
	} else {
		throw exception("Couldn't load sprite: couldn't detect texture format");
	}

	w = surface->w;
	h = surface->h;
 
	// Have OpenGL generate a texture object handle for us
	glGenTextures( 1, &textureId );
 
	// Bind the texture object
	glBindTexture( GL_TEXTURE_2D, textureId );
 
	// Set the texture's stretching properties
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
 
	// Edit the texture object's image data using the information SDL_Surface gives us
	glTexImage2D( GL_TEXTURE_2D, 0, nOfColors, surface->w, surface->h, 0,
						texture_format, GL_UNSIGNED_BYTE, surface->pixels );

	if (surface) { 
		SDL_FreeSurface(surface);
	}	
}

GLuint Texture::id() const
{
	return textureId;
}

GLuint Texture::width() const
{
	return textureId;
}

GLuint Texture::height() const
{
	return textureId;
}

void Texture::glTexCoord(int x, int y) const
{
	glTexCoord2f(x/(GLfloat)w, y/(GLfloat)h);
}

void glBindTexture(const Texture& t)
{
	glBindTexture( GL_TEXTURE_2D, t.id());
}