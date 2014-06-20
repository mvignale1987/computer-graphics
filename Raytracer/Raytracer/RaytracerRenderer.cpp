#include "RaytracerRenderer.h"
#include <windows.h>
#include <ctime>
#include <SDL_opengl.h>
#include <GL/GLU.h>
#include <IL/devil_cpp_wrapper.hpp>
#include "Scene.h"
#include "App.h"
#include "SceneError.h"

using namespace std;

const unsigned char RaytracerRenderer::superSamplingConstant = 2;

BufferContent::BufferContent(unsigned char r, unsigned char g, unsigned char b, unsigned char a):
	r(r), g(g), b(b), a(a)
{
}

BufferContent::BufferContent(const Vector3& color, unsigned char a):
	a(a)
{
	float r = color.x();
	float g = color.y();
	float b = color.z();
	
	if(r < 0)
		r = 0;
	if(r > 1)
		r = 1;
	this->r = (unsigned char) (r * 256.0f);

	if(g < 0)
		g = 0;
	if(g > 1)
		g = 1;
	this->g = (unsigned char) (g * 256.0f);

	if(b < 0)
		b = 0;
	if(b > 1)
		b = 1;
	this->b = (unsigned char) (b * 256.0f);
}

RaytracerRenderer::RaytracerRenderer(App &app, Scene &scene):
	Renderer(app, scene),
	bufferSize(scene.imageWidth() * scene.imageHeight() * superSamplingConstant * superSamplingConstant),
	colorBuffer(new BufferContent[bufferSize]),
	colorBufferChanged(true),
	status(RaytracerStatus::INIT)
{
	// initialize image to sample output color
	for(size_t i = 0; i < bufferSize; ++i)
	{
		colorBuffer[i] = BufferContent(scene.backgroundColor());
	}
	status = RaytracerStatus::RENDER_DONE;
}

void RaytracerRenderer::init()
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glClearColor(Vector3::zero);
	glEnable(GL_TEXTURE_2D);
	glShadeModel(GL_SMOOTH);		 // Enables Smooth Shading
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculation

	glGenTextures(1, &colorTextureId);
    // Use this texture id
    glBindTexture(GL_TEXTURE_2D, colorTextureId);
    // Associate this texture id with some texture-specific data
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);


}

void RaytracerRenderer::handleReshape(int width, int height)
{
	if (height==0)
	{
		height=1;
	}
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-width, width, -height, height);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	renderColorBuffer();
}

void RaytracerRenderer::renderNextFrame()
{
	if(colorBufferChanged)
	{
		renderColorBuffer();
		colorBufferChanged = false;
	}
	if(status == RaytracerStatus::RENDER_DONE)
	{
		saveImage();
		status = RaytracerStatus::SAVED;
	}
}

void RaytracerRenderer::renderColorBuffer()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	int w = scene().imageWidth() * superSamplingConstant;
	int h = scene().imageHeight() * superSamplingConstant;
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, colorBuffer);

	glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	// One vertex is (2 + 2)*4 bytes (2 GLfloats for position, 2 GLfloats for texture coordinates, 4 bytes per GLfloat)
    const int SIZE_OF_VERTEX = (2 + 2)*sizeof(GLfloat);

    GLfloat data[] = {// 2 texture coordinates followed by 2 vertex coordinates in counterclockwise order
        0, 0, -1, -1,// Bottomleft
        1, 0, +1, -1,// Bottomright
        1, 1, +1, +1,// Topright
        0, 1, -1, +1,// Topleft
    };

    // Use this texture
	glBindTexture(GL_TEXTURE_2D, colorTextureId);

    // Tell OpenGL where our vertex data begins and how big each vertex is
    glTexCoordPointer(2, GL_FLOAT, SIZE_OF_VERTEX, data+0);// Texture data begins at index 0
    glVertexPointer  (2, GL_FLOAT, SIZE_OF_VERTEX, data+2);// Vertex  data begins at index 2

	int winWidth, winHeight;
	SDL_GetWindowSize(app().getWindow(), &winWidth, &winHeight);

	GLfloat imageAspectRatio = (GLfloat) scene().imageWidth() / (GLfloat) scene().imageHeight();
	GLfloat windowAspectRatio = (GLfloat) winWidth / (GLfloat) winHeight;

	// makes image fit in window
	if(imageAspectRatio > windowAspectRatio)
	{
		glScalef((GLfloat)winWidth, winHeight * windowAspectRatio / imageAspectRatio, 1.0f);
	} else {
		glScalef(winWidth * imageAspectRatio / windowAspectRatio, (GLfloat)winHeight, 1.0f);
	}

    // Draw data
    glDrawArrays(GL_QUADS, 0, 4);
}

void RaytracerRenderer::saveImage()
{
	static const int maxTimeLength = 255;
	char timeStr[maxTimeLength];
	time_t rawtime;
	time (&rawtime);
	tm timeinfo;
	localtime_s(&timeinfo, &rawtime);
	strftime (timeStr, maxTimeLength, "%Y-%m-%d %H.%M.%S", &timeinfo);
	string outPath = scene().outputDir() + '\\' + timeStr + ".png";

	CreateDirectory(scene().outputDir().c_str(), NULL);

	ILuint imageID = ilGenImage();
	ilBindImage(imageID);
	ilTexImage(
		scene().imageWidth() * superSamplingConstant,
		scene().imageHeight() * superSamplingConstant,
		1,  // OpenIL supports 3d textures!  but we don't want it to be 3d. so we just set this to be 1
		4,  //  channels: one for R , one for G, one for B, one for A
		IL_RGBA,  // duh, yeah use rgba
		IL_UNSIGNED_BYTE,  // the type of data the imData array contains (next)
		colorBuffer  // and the array of bytes represneting the actual image data
	);
	ilEnable(IL_FILE_OVERWRITE);
	iluScale(scene().imageWidth(), scene().imageHeight(), 1);
	ilSave( IL_PNG, outPath.c_str());

	ILenum ilError = ilGetError();
	if(ilError != IL_NO_ERROR)
	{
		throw SceneError::fromILError(ilError, "Error while saving image");
	}
} 