#include "Skybox.h"
#include "Vector3.h"
#include <GL/GLU.h>

Skybox::Skybox(Camera *camera):
	texture("skybox.png", false, true),
	camera(camera)
{
}

void Skybox::render(Scene &parent)
{
   /*// Store the current matrix
   glPushMatrix();

   // Reset and transform the matrix.
   glLoadIdentity();
   gluLookAt(
       0,0,0,
       camera->x(),camera->y(),camera->z(),
       0,1,0);

   // Enable/Disable features
   glPushAttrib(GL_ENABLE_BIT);
   glEnable(GL_TEXTURE_2D);
   glDisable(GL_DEPTH_TEST);
   glDisable(GL_LIGHTING);
   glDisable(GL_BLEND);

   // Just in case we set all vertices to white.
   glColor4f(1,1,1,1);

   const float deltaY = 0.5f;
   const float deltaX = 0.25f;

   // Render the front quad
   glBindTexture(texture);
   glBegin(GL_QUADS);
   {
       glTexCoord2f(deltaX*3, deltaY*2); glVertex3f(  0.5f, -0.5f, -0.5f );
       glTexCoord2f(1,		  deltaY*2); glVertex3f( -0.5f, -0.5f, -0.5f );
       glTexCoord2f(1,		  deltaY); glVertex3f( -0.5f,  0.5f, -0.5f );
       glTexCoord2f(deltaX*3, deltaY); glVertex3f(  0.5f,  0.5f, -0.5f );
   }
   glEnd();

   // Render the left quad
   glBegin(GL_QUADS);
   {
       glTexCoord2f(deltaX*2, deltaY*2); glVertex3f(  0.5f, -0.5f,  0.5f );
       glTexCoord2f(deltaX*3, deltaY*2); glVertex3f(  0.5f, -0.5f, -0.5f );
       glTexCoord2f(deltaX*3, deltaY); glVertex3f(  0.5f,  0.5f, -0.5f );
       glTexCoord2f(deltaX*2, deltaY); glVertex3f(  0.5f,  0.5f,  0.5f );
   }
   glEnd();

   // Render the back quad
   glBegin(GL_QUADS);
   {
       glTexCoord2f(deltaX, deltaY*2); glVertex3f( -0.5f, -0.5f,  0.5f );
       glTexCoord2f(deltaX*2, deltaY*2); glVertex3f(  0.5f, -0.5f,  0.5f );
       glTexCoord2f(deltaX*2, deltaY); glVertex3f(  0.5f,  0.5f,  0.5f );
       glTexCoord2f(deltaX, deltaY); glVertex3f( -0.5f,  0.5f,  0.5f );
   }
   glEnd();

   // Render the right quad
   glBegin(GL_QUADS);
   {
       glTexCoord2f(0,		deltaY*2);		glVertex3f( -0.5f, -0.5f, -0.5f );
       glTexCoord2f(deltaX, deltaY*2); glVertex3f( -0.5f, -0.5f,  0.5f );
       glTexCoord2f(deltaX, deltaY); glVertex3f( -0.5f,  0.5f,  0.5f );
       glTexCoord2f(0,		deltaY); glVertex3f( -0.5f,  0.5f, -0.5f );
   }
   glEnd();

   // Render the top quad
   glBegin(GL_QUADS);
   {
		glTexCoord2f(deltaX*3, deltaY); glVertex3f( -0.5f,  0.5f, -0.5f );
		glTexCoord2f(deltaX*3, 0); glVertex3f( -0.5f,  0.5f,  0.5f );
		glTexCoord2f(1,		   0); glVertex3f(  0.5f,  0.5f,  0.5f );
		glTexCoord2f(1,        deltaY); glVertex3f(  0.5f,  0.5f, -0.5f );
   }
   glEnd();

   // Restore enable bits and matrix
   glPopAttrib();
   glPopMatrix();*/

}