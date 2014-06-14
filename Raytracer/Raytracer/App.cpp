#include "App.h"
#include "Logger.h"
#include "Renderer.h"
#include "SceneError.h"
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <sstream>
#include <GL/freeglut.h>

using namespace std;

const std::string App::windowTitle = "RayTracer";
const std::string App::appIconPath = "icon.png";

App::App():
	win(nullptr),
	ren(nullptr),
	glContext(0),
	frameTime(0.0f),
	nFrames(0)
{
}

void App::setRenderer(Renderer *r)
{
	renderer = r;
}

Renderer *App::getRenderer()
{
	return renderer;
}

int App::initWindow()
{
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0){
		Logger::logSDLError("SDL_Init Error");
		return 1;
	}

	// Settings for antialiasing
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);
    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
    glEnable(GL_MULTISAMPLE);

	/* Turn on double buffering with a 24bit Z buffer.
     * You may need to change this to 16 or 32 for your system */
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

	// Creat window, renderer and GL context
	win = SDL_CreateWindow(
		windowTitle.c_str(),
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		800, 600,
		SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE 
	);
	if (win == nullptr){
		Logger::logSDLError("SDL_CreateWindow Error");
		return 1;
	}
	ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (ren == nullptr){
		Logger::logSDLError("SDL_CreateRenderer Error");
		return 1;
	}
	glContext = SDL_GL_CreateContext(win);
	if(glContext == nullptr)
	{
		Logger::logSDLError("SDL_GL_CreateContext Error");
		return 1;
	}

	// load app icon
	SDL_Surface *image;
	image = IMG_Load(appIconPath.c_str());
	if(!image) {
		stringstream ss;
		ss << "Loading App icon Error: IMG_Load: " << IMG_GetError();
		Logger::log(ss.str());
	} else {
		SDL_SetWindowIcon(win, image); 
		SDL_FreeSurface(image);
	}

	setVSync(true);

	return 0;
}

bool App::pollEvent()
{
	while(SDL_PollEvent(NULL) > 0)
	{
		SDL_Event e;
		SDL_PollEvent(&e);
		switch (e.type)  {
			case SDL_WINDOWEVENT:
				switch (e.window.event)  {   
					case SDL_WINDOWEVENT_SIZE_CHANGED: 
						renderer->handleReshape( e.window.data1, e.window.data2);
						return true;
					default:
						return renderer->handleEvent(e);
				}
			case SDL_QUIT:
				return false;
			default:
				return renderer->handleEvent(e);
		}
	}
	return true;
}

void App::clean()
{
	renderer->clean();
	checkOpenGLError("Renderer clean");
}

void App::renderNextFrame()
{
	++nFrames;
	Uint32 start = SDL_GetTicks();
	renderer->renderNextFrame();
	checkOpenGLError("renderNextFrame");
	SDL_GL_SwapWindow(win);
	frameTime = (SDL_GetTicks() - start) / 1000.0f;
}

int App::run(int argc, char **argv)
{
	int retcode;
	glutInit(&argc, argv);
	
	retcode = initWindow();
	if(retcode != 0)
		return retcode;
		
	try {
		renderer->init();
		checkOpenGLError("Renderer init");
		int w, h;
		SDL_GetWindowSize(win, &w, &h);
		renderer->handleReshape(w, h);
		checkOpenGLError("Initial Render reshape");
	
		while (pollEvent()){
			unsigned int windowFlags = SDL_GetWindowFlags(win);
			if(windowFlags & (SDL_WINDOW_HIDDEN | SDL_WINDOW_MINIMIZED)){
				continue;
			}
			renderNextFrame();
		}

		clean();
		retcode = 0;
	} catch(exception& ex)
	{
		Logger::logError(ex.what());
		retcode = 1;
	}

	SDL_DestroyWindow(win);
	SDL_Quit();
	return retcode;
}

SDL_Window *App::getWindow() const
{
	return win;
}

SDL_Renderer *App::getRenderer() const
{
	return ren;
}

SDL_GLContext App::getGlContext() const
{
	return glContext;
}

float App::getFrameTime() const
{
	return frameTime;
}

unsigned int App::getRenderedFrames() const
{
	return nFrames;
}


void App::setVSync(bool enabled)
{
	//Use Vsync
	if(SDL_GL_SetSwapInterval(enabled ? 1: 0) < 0 ){
		Logger::logSDLError("SDL_GL_SetSwapInterval Error");
	}
}

void App::checkOpenGLError(const string message)
{
	GLenum error = glGetError();
	if(error != GL_NO_ERROR)
		throw SceneError::fromGLError(error, message);
}
