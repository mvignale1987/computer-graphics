#include "OpenGLRenderer.h"
#include <SDL_opengl.h>
#include <GL/GLU.h>
#include <GL/freeglut.h>
#include "Scene.h"
#include "SceneObjectSphere.h"
#include "SceneObjectQuad.h"
#include "SceneObjectTriangle.h"
#include "SceneObjectCyllinder.h"

using namespace std;

OpenGLRenderer::OpenGLRenderer(App &app, Scene &scene):
	Renderer(app, scene)
{
}

void OpenGLRenderer::init()
{
	glEnable(GL_DEPTH_TEST);
	glClearColor(scene().backgroundColor());
	glShadeModel(GL_SMOOTH);		
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	

	glEnable(GL_LIGHTING);
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
	glEnable(GL_NORMALIZE);


	vector<Light>& lights = scene().lights();
	for(unsigned int i = 0; i < lights.size(); ++i)
	{
		GLenum lightEnum;

		if(i == 0)
			lightEnum = GL_LIGHT0;
		else if(i == 1)
			lightEnum = GL_LIGHT1;
		else if(i == 2)
			lightEnum = GL_LIGHT2;
		else if(i == 4)
			lightEnum = GL_LIGHT3;
		else if(i == 5)
			lightEnum = GL_LIGHT4;
		else if(i == 6)
			lightEnum = GL_LIGHT5;
		else if(i == 7)
			lightEnum = GL_LIGHT6;
		else if(i == 8)
			lightEnum = GL_LIGHT7;
		else
			break;

		glEnable(lightEnum);
		Light light = lights[i];
		glLight(lightEnum, GL_AMBIENT, light.ambientColor() , 1);
		glLight(lightEnum, GL_DIFFUSE, light.diffuseColor(), 1);
		glLight(lightEnum, GL_POSITION, -light.position(), 1);
		glLightf(lightEnum, GL_CONSTANT_ATTENUATION, 0);
		glLightf(lightEnum, GL_LINEAR_ATTENUATION, light.linearAttenuation() / 5.0f);
		glLightf(lightEnum, GL_QUADRATIC_ATTENUATION, light.quadAttenuation() / 5.0f);
	}
}

void OpenGLRenderer::handleReshape(int width, int height )
{
	if (height==0)
	{
		height=1;
	}
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	Camera cam = getCorrectedCamera();

	gluPerspective(cam.fov(),(GLfloat)width/(GLfloat)height, cam.zNear(), cam.zFar());
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

Camera OpenGLRenderer::getCorrectedCamera()
{
	Camera cam = scene().camera();
	Vector3 position = cam.position().scale(1.0f, 0.5f, 1.0f);
	Vector3 lookAt = cam.lookAt().scale(1.0f, 0.5f, 1.0f);
	return Camera(position, lookAt, cam.up(), cam.zNear(), cam.zFar(), cam.fov());
}

void OpenGLRenderer::renderNextFrame()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	Camera cam = getCorrectedCamera();
	gluLookAt(cam.position(), cam.lookAt(), cam.up());

	glScalef(-1.0f, 1.0f, 1.0f);
	glTranslate(-cam.position());

	vector<SceneObject *> objects = scene().objects();
	for(vector<SceneObject *>::iterator it = objects.begin(); it != objects.end(); ++it)
	{
		Material& material = *(*it)->material();

		glMaterial(GL_AMBIENT, material.ambientColor(), material.ambientCoefficient());
		glMaterial(GL_DIFFUSE, material.diffuseColor(), material.diffuseCoefficient());
		glMaterial(GL_SPECULAR, material.specularColor(), material.specularCoefficient());
		glMaterial(GL_EMISSION, Vector3::zero, 0);
		glMateriali(GL_FRONT_AND_BACK, GL_SHININESS, material.specularExponent());

		SceneObjectSphere *sphere = dynamic_cast<SceneObjectSphere *> (*it);
		if(sphere){
			renderSphere(sphere);
			continue;
		}
		SceneObjectCyllinder *cyllinder = dynamic_cast<SceneObjectCyllinder *> (*it);
		if(cyllinder){
			renderCyllinder(cyllinder);
			continue;
		}
		SceneObjectQuad *quad = dynamic_cast<SceneObjectQuad *> (*it);
		if(quad){
			renderQuad(quad);
			continue;
		}
		SceneObjectTriangle *triangle = dynamic_cast<SceneObjectTriangle *> (*it);
		if(triangle){
			renderTriangle(triangle);
			continue;
		}
	}
}


void OpenGLRenderer::renderSphere(SceneObjectSphere *sphere)
{
	glPushMatrix();
	{
		glTranslate(sphere->center());
		glutSolidSphere(sphere->radius(), 32, 32);
	}
	glPopMatrix();
}

void OpenGLRenderer::renderCyllinder(SceneObjectCyllinder *cyllinder)
{
	glPushMatrix();
	{
		glTranslate(cyllinder->center());
		glRotate(90.0f, Vector3::left);
		glutSolidCylinder(cyllinder->radius(), cyllinder->height(), 32, 32);
	}
	glPopMatrix();
}

void OpenGLRenderer::renderQuad(SceneObjectQuad *quad)
{
	glPushMatrix();
	glBegin(GL_QUADS);
	{
		vector<Vector3>& vertexs = quad->vertexs();
		for(vector<Vector3>::iterator it = vertexs.begin(); it != vertexs.end(); ++it)
		{
			glNormal(quad->normal());
			glVertex(*it);
		}
	}
	glEnd();
	glPopMatrix();
}

void OpenGLRenderer::renderTriangle(SceneObjectTriangle *triangle)
{
	glPushMatrix();
	glBegin(GL_TRIANGLES);
	{
		glNormal(triangle->normal());
		glVertex(triangle->a());
		glVertex(triangle->b());
		glVertex(triangle->c());
	}
	glEnd();
	glPopMatrix();
}