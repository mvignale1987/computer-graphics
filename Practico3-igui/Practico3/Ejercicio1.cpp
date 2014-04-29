#include <iostream>
#include "Scene.h"
#include "Vector3.h"
#include <GL/GLU.h>
#include <GL/GL.h>
#include <vector>
#include <sstream>
#include <fstream>
#include <ctime>

using namespace std;

class TriangleTree {
public:
	const Vector3 position;
	const Vector3 color;

	TriangleTree(Vector3 position, Vector3 color):
		position(position),
		color(color)
	{
	}
};



class EjScene: public Scene {
private:
	// for displaying trees
	GLuint treeList; GLuint centerList; GLuint floorList;
	vector<TriangleTree> trees;
	const int nTrees;
	const int treeHeight;
	GLfloat angle;

	// camera position
	Vector3 cameraPosition;
	GLfloat headingHorizontal; // clamped to [0, 360]
	GLfloat headingVertical;  // clamped to [-90, 90]
	const GLfloat cameraSpeed; // world units / s
public:
	EjScene():
		treeList(0),
		nTrees(100),
		treeHeight(7),
		angle(0),
		headingHorizontal(0),
		headingVertical(0), 
		cameraSpeed(1.5)
	{
	}

	void init()
	{
		srand((unsigned int)time(NULL));

		for(int i = 0; i < nTrees; ++i)
		{
			GLfloat positionX = 50 * rand() / (GLfloat) RAND_MAX - 25;
			GLfloat positionZ = 50 * rand() / (GLfloat) RAND_MAX - 25;
			Vector3 position(positionX, 0, positionZ);
			trees.push_back(TriangleTree(position, Vector3::random()));
		}

		treeList = glGenLists (3);
		glNewList (treeList, GL_COMPILE);
		tree();
		glEndList ();

		centerList = treeList + 1;
		glNewList(centerList, GL_COMPILE);
		center();
		glEndList();

		floorList = treeList + 2;
		glNewList(floorList, GL_COMPILE);
		floor();
		glEndList();


		glShadeModel (GL_FLAT);
		glEnable(GL_DEPTH_TEST);
	}

	void reshape(int w, int h)
	{
		glViewport(0, 0, w, h);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(60, (GLfloat)w  / (GLfloat)h, 0.1, 100);
	}

	void moveForward(GLfloat ratio)
	{
		GLfloat delta = cameraSpeed * getFrameTime() * ratio;
		Vector3 headingVector(
			sinf(-headingHorizontal *  (GLfloat) M_PI / 180.0f),
			0.0f,
			cosf(-headingHorizontal *  (GLfloat) M_PI / 180.0f)
		);

		cameraPosition += delta * headingVector;
	}

	void moveLateral(GLfloat ratio)
	{
		GLfloat delta = cameraSpeed * getFrameTime() * ratio;
		Vector3 headingVector(
			cosf(headingHorizontal * (GLfloat)M_PI / 180.0f),
			0.0f,
			sinf(headingHorizontal * (GLfloat)M_PI / 180.0f)
		);

		cameraPosition += delta * headingVector;
	}

	void rotateY(GLfloat ratio)
	{
		GLfloat delta = cameraSpeed * getFrameTime() * ratio;
		headingHorizontal += ratio;
		if(headingHorizontal > 360.0)
			headingHorizontal -= 360;
		else if(headingHorizontal < 0){
			headingHorizontal += 360.0;
		}		
	}

	bool handleEvent(SDL_Event ev)
	{
		const Uint8 *state = SDL_GetKeyboardState(NULL);
		GLfloat ratio = 1;
		if(state[SDL_SCANCODE_LSHIFT] || state[SDL_SCANCODE_RSHIFT]){
			ratio *= 3;
		}

		if (state[SDL_SCANCODE_W]) {
			moveForward(ratio);
		} else if(state[SDL_SCANCODE_S]) {
			moveForward(-ratio);
		}
		if (state[SDL_SCANCODE_A]) {
			moveLateral(ratio);
		} else if(state[SDL_SCANCODE_D]) {
			moveLateral(-ratio);
		}
		if (state[SDL_SCANCODE_Q]) {
			rotateY(-ratio);
		} else if(state[SDL_SCANCODE_E]) {
			rotateY(ratio);
		} else if(state[SDL_SCANCODE_R]) {
			headingHorizontal = 0;
			cameraPosition = Vector3();
		}
		return true;
	}

	void render()
	{
		angle += 360.0f * getFrameTime() * 0.02f;
		if(angle > 360)
			angle -= 360.0;

		glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		if(getRenderedFrames() %  30 == 0){
			stringstream ss;
			ss << "Horizontal: " << headingHorizontal;
			log(ss.str());
			ss.clear();
			ss << "Position: " << cameraPosition << endl;
			log(ss.str());
		}

		glRotate(headingHorizontal, Vector3::up);
		glTranslate(cameraPosition);
		

		glCallList(centerList);
		glCallList(floorList);

		for (int i = 1; i < nTrees; i++) {  
			glPushMatrix();
			{
				TriangleTree &tree = trees.at(i);
				glTranslate(tree.position);
				glRotate(-angle * treeHeight, Vector3::up);
				for(int j = 0; j < treeHeight; ++j){
					glPushMatrix();
					glRotate(angle * treeHeight, Vector3::up);
				}
				glColor(tree.color);
				glCallList (treeList);
			}
			glPopMatrix();
		}
		glFlush ();
	}

	void tree()
	{
		glPushMatrix();
		for(int i = treeHeight-1; i >= 0; --i)
		{
			glPopMatrix();
			glTranslatef(0, (GLfloat)i, 0);
			glBegin (GL_TRIANGLES);
			{
				glVertex2f (-0.5, -0.5);
				glVertex2f ( 0.5, -0.5);
				glVertex2f (0.0, 0.5);
			}
			glEnd ();
		}
		glPopMatrix();
	}

	void center()
	{
		glPointSize(15);
		glBegin(GL_POINTS);
		{
			glColor(Vector3::one);
			glVertex(Vector3::zero);
		}
		glEnd();
	}

	void floor()
	{
		glBegin(GL_QUADS);
		{
			glColor3f(0.1f,0.1f,1.0f);
			glVertex3f(-5000, -1, -5000);
			glVertex3f(-5000, -1, 5000);
			glVertex3f(5000, -1, 5000);
			glVertex3f(5000, -1, -5000);
		}
		glEnd();
	}

	void clean()
	{
	}
};


int main(int argc, char **argv)
{
	return EjScene().run();
}