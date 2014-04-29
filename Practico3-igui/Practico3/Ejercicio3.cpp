#include "Scene.h"
#include <GL/GLU.h>
#include <GL/GL.h>
#include <GL/freeglut.h>
#include "Texture.h"
#include "Model.h"
#include "Vector3.h"
#include "Quaternion.h"
#include <ctime>


class EjScene: public Scene {
private:
	// currently these are hardcoded
	Model model;
	GLuint floorDisplayList;
	GLuint modelDisplayList;
	const GLfloat floorSize;
	Vector3 position;
	Vector3 velocity;
	Quaternion rotation;
	GLfloat rotX, rotY;
	const GLfloat bounciness; // should be (0, 1] 1 = perfect elasitic, 0 = inelastic (stucks to surface)
public:
	EjScene():
		floorDisplayList(-1),
		modelDisplayList(-1),
		floorSize(3),
		rotX(0), rotY(0),
		bounciness(0.7f)
	{
	}

	void init()
	{
		srand((unsigned)time(NULL));
		model.load("pinchos.3ds");

		glEnable(GL_TEXTURE_2D);
		glShadeModel(GL_SMOOTH);		 // Enables Smooth Shading
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClearDepth(1.0f);				// Depth Buffer Setup
		glEnable(GL_DEPTH_TEST);		// Enables Depth Testing
		glDepthFunc(GL_LEQUAL);			// The Type Of Depth Test To Do
		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculation


		enableLighting();
		
		floorDisplayList = glGenLists(2);
		glNewList (floorDisplayList, GL_COMPILE);
		{
			renderFloor();
		}
		glEndList();

		modelDisplayList = floorDisplayList + 1;
		glNewList (modelDisplayList, GL_COMPILE);
		{
			renderModel();
		}
		glEndList();

		resetScene();
	}

	bool handleEvent(SDL_Event ev)
	{
		if(ev.type == SDL_KEYDOWN)
		{
			switch(ev.key.keysym.scancode)
			{
				case SDL_SCANCODE_SPACE:
					resetScene();
					break;
			}
		}

		const Uint8 *state = SDL_GetKeyboardState(NULL);
		GLfloat ratio = getFrameTime() * 50;
		if(state[SDL_SCANCODE_LSHIFT] || state[SDL_SCANCODE_RSHIFT]){
			ratio *= 3;
		}
		if (state[SDL_SCANCODE_W]) {
			rotX += ratio;
		} else if(state[SDL_SCANCODE_S]) {
			rotX -= ratio;
		}
		if (state[SDL_SCANCODE_A]) {
			rotY -= ratio;
		} else if(state[SDL_SCANCODE_D]) {
			rotY += ratio;
		}
		
		return true;
	}


	void enableLighting()
	{
		GLfloat LightAmbient[4] = { 0.5f, 0.5f, 0.5f, 1.0f };
		GLfloat LightDiffuse[4] = { 1, 1, 1, 1};
		GLfloat LightPosition[4] = { 0, 0, 15, 1 };


		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);    // Uses default lighting parameters
		glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
		glEnable(GL_NORMALIZE);

		glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);
		glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);
		glLightfv(GL_LIGHT1, GL_POSITION, LightPosition);
		glEnable(GL_LIGHT1);
	}

	void reshape(int width, int height)
	{
		if (height==0)
		{
			height=1;
		}
		glViewport(0, 0, width, height);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(45.0f,(GLfloat)width/(GLfloat)height, 0.1f, 100.0f);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
	}

	void resetScene()
	{
		rotX = 30;
		rotY = 45;
		position = Vector3::random().scale(3, 1, 3) - Vector3(1.5f, 0, 1.5f);
		velocity = Vector3::random().scale(4, 4, 4) + Vector3::up * 3;
		rotation = Quaternion(1, 1, 1, 1);
	}

	void calcPosition()
	{
		static const Vector3 g(0, 9.8f, 0);

		position += velocity * getFrameTime();
		velocity -= g * getFrameTime();
		
		rotation = rotation.lerp(rotation, Quaternion(-g.cross(rotation.eulerAngles())), getFrameTime());

		if(position.y() < 0)
		{
			position = Vector3(position.x(), 0, position.z());
			velocity = velocity.scale(1, -bounciness, 1);
		}

		GLfloat limitXZ = floorSize - 0.5f;
		if(fabsf(position.x()) > limitXZ){
			int sign = position.x() > 0 ? 1 : -1;
			position = Vector3(limitXZ * sign, position.y(), position.z());
			velocity = velocity.scale(-bounciness, 1, 1);
		}
		if(fabsf(position.z()) > limitXZ){
			int sign = position.z() > 0 ? 1 : -1;
			position = Vector3(position.x(), position.y(), limitXZ * sign);
			velocity = velocity.scale(1, 1, -bounciness);
		}
	}

	void render()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glLoadIdentity();
		glTranslatef(0, 1, -10);
		glRotatef(rotX, 1, 0, 0);
		glRotatef(rotY, 0, 1, 0);

		calcPosition();
		
		glPushMatrix();
		{
			glTranslate(position);
			glRotate(rotation);
			glCallList(modelDisplayList);
		}
		glPopMatrix();
		glCallList(floorDisplayList);
	}

	void renderFloor()
	{
		glDisable(GL_TEXTURE_2D);
		glBegin(GL_QUADS);
		{
			glColor3f(1,1,1);
			glVertex3f(-floorSize, -1, -floorSize);
			glVertex3f(-floorSize, -1,  floorSize);
			glVertex3f( floorSize, -1,  floorSize);
			glVertex3f( floorSize, -1, -floorSize);
		}
		glEnd();
		glEnable(GL_TEXTURE_2D);
	}

	void renderModel()
	{
		glDisable(GL_LIGHTING);
		glDisable(GL_TEXTURE_2D);
		glColor(Vector3::random());
		//glutWireSphere(1, 16, 16);
		glEnable(GL_LIGHTING);
		glEnable(GL_TEXTURE_2D);
		model.render(0.65f);
	}

	void clean()
	{
	}
};


int main(int argc, char **argv)
{
	return EjScene().run();
}