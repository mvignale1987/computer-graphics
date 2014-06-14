#include "OpenGLRenderer.h"
#include <SDL_opengl.h>
#include <GL/GLU.h>

OpenGLRenderer::OpenGLRenderer(App &app, Scene &scene):
	Renderer(app, scene)
{
}

void OpenGLRenderer::handleReshape(int height, int width)
{
	if (height==0)
	{
		height=1;
	}
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f,(GLfloat)width/(GLfloat)height, 0.1f, 2000.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void OpenGLRenderer::renderNextFrame()
{
}