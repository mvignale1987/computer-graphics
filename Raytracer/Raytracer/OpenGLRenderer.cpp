#include "OpenGLRenderer.h"
#include <SDL_opengl.h>
#include <GL/GLU.h>
#include "Scene.h"

OpenGLRenderer::OpenGLRenderer(App &app, Scene &scene):
	Renderer(app, scene)
{
}

void OpenGLRenderer::init()
{
	glEnable(GL_DEPTH_TEST);
	// blend
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glClearColor(scene().backgroundColor());
	glEnable(GL_TEXTURE_2D);
	glShadeModel(GL_SMOOTH);		 // Enables Smooth Shading
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculation

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
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}