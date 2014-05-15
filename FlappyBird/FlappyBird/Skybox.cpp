#include "Skybox.h"
#include "Vector3.h"
#include <GL/GLU.h>

Skybox::Skybox(Camera *camera):
	texture("skybox.png", true, true),
	camera(camera)
{
}

void Skybox::render(Scene &parent)
{
   // Store the current matrix
   glPushMatrix();

   // Reset and transform the matrix.
   glLoadIdentity();
   gluLookAt(
       0,0,0,
       camera->x(),camera->y(),camera->z(),
       0,-1,0);

   // Enable/Disable features
   glPushAttrib(GL_ENABLE_BIT);
   glEnable(GL_TEXTURE_2D);
   glDisable(GL_DEPTH_TEST);
   glDisable(GL_LIGHTING);
   glDisable(GL_BLEND);

   // Just in case we set all vertices to white.
   glColor4f(1,1,1,1);
   const double deltaX = 0.25;

   // Render the front quad
   glBindTexture(texture);
   glBegin(GL_QUADS);
   {
       glTexCoord2d(deltaX*3, 0); glVertex3f(  0.5f, -0.5f, -0.5f );
       glTexCoord2d(1,		  0); glVertex3f( -0.5f, -0.5f, -0.5f );
       glTexCoord2d(1,		  1); glVertex3f( -0.5f,  0.5f, -0.5f );
       glTexCoord2d(deltaX*3, 1); glVertex3f(  0.5f,  0.5f, -0.5f );
   }
   glEnd();

   // Render the left quad
   glBegin(GL_QUADS);
   {
       glTexCoord2d(deltaX*2, 0); glVertex3f(  0.5f, -0.5f,  0.5f );
       glTexCoord2d(deltaX*3, 0); glVertex3f(  0.5f, -0.5f, -0.5f );
       glTexCoord2d(deltaX*3, 1); glVertex3f(  0.5f,  0.5f, -0.5f );
       glTexCoord2d(deltaX*2, 1); glVertex3f(  0.5f,  0.5f,  0.5f );
   }
   glEnd();

   // Render the back quad
   glBegin(GL_QUADS);
   {
       glTexCoord2d(deltaX, 0); glVertex3f( -0.5f, -0.5f,  0.5f );
       glTexCoord2d(deltaX*2, 0); glVertex3f(  0.5f, -0.5f,  0.5f );
       glTexCoord2d(deltaX*2, 1); glVertex3f(  0.5f,  0.5f,  0.5f );
       glTexCoord2d(deltaX, 1); glVertex3f( -0.5f,  0.5f,  0.5f );
   }
   glEnd();

   // Render the right quad
   glBegin(GL_QUADS);
   {
       glTexCoord2d(0,		0);		glVertex3f( -0.5f, -0.5f, -0.5f );
       glTexCoord2d(deltaX, 0); glVertex3f( -0.5f, -0.5f,  0.5f );
       glTexCoord2d(deltaX, 1); glVertex3f( -0.5f,  0.5f,  0.5f );
       glTexCoord2d(0,		1); glVertex3f( -0.5f,  0.5f, -0.5f );
   }
   glEnd();


   // Restore enable bits and matrix
   glPopAttrib();
   glPopMatrix();

}