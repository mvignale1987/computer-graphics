#include "Scene.h"
#include "SceneError.h"
#include <SDL_opengl.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <ctime>
#include <SDL_image.h>
#include <assimp/DefaultLogger.hpp>
#include <GL/freeglut.h>

using namespace std;

Scene::Scene():
	win(nullptr),
	ren(nullptr),
	glContext(0),
	frameTime(0.0f),
	nFrames(0)
{
}

void Scene::init()
{
}

bool Scene::handleEvent(SDL_Event ev)
{
	return true;
}

void Scene::render()
{
}

void Scene::clean()
{
}

Scene::~Scene()
{
}

void Scene::reshape(int, int)
{
}

void Scene::logSDLError(const string &prefix)
{
	string msg = prefix + ": " + SDL_GetError();
	
}

string Scene::logFileName()
{
	return "log.txt";
}

string Scene::windowTitle()
{
	return "App";
}

void Scene::logError(const string &msg)
{
	MessageBox(NULL, msg.c_str(), (windowTitle() + " :: Error").c_str(), MB_OK | MB_ICONERROR);
}

void Scene::initAssimpLog()
{
	Assimp::Logger::LogSeverity severity = Assimp::Logger::VERBOSE;
	Assimp::DefaultLogger::create("assimp_log.txt",severity, aiDefaultLogStream_FILE);
}

void Scene::cleanAssimpLog()
{
	// Kill it after the work is done
	Assimp::DefaultLogger::kill();
}

int Scene::initWindow()
{
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0){
		logSDLError("SDL_Init Error");
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
		windowTitle().c_str(),
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		640, 480,
		SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE 
	);
	if (win == nullptr){
		logSDLError("SDL_CreateWindow Error");
		return 1;
	}
	ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (ren == nullptr){
		logSDLError("SDL_CreateRenderer Error");
		return 1;
	}
	glContext = SDL_GL_CreateContext(win);
	if(glContext == nullptr)
	{
		logSDLError("SDL_GL_CreateContext Error");
		return 1;
	}

	// load app icon
	SDL_Surface *image;
	image = IMG_Load(appIconPath().c_str());
	if(!image) {
		stringstream ss;
		ss << "Loading App icon Error: IMG_Load: " << IMG_GetError();
		log(ss.str());
	} else {
		SDL_SetWindowIcon(win, image); 
		SDL_FreeSurface(image);
	}

	//Use Vsync
	if(SDL_GL_SetSwapInterval(1) < 0 ){
		logSDLError("SDL_GL_SetSwapInterval Error");
		return 1;
	}

	return 0;
}

bool Scene::pollEvent()
{
	SDL_Event e;
	SDL_PollEvent(&e);
	switch (e.type)  {
        case SDL_WINDOWEVENT:
			switch (e.window.event)  {   
				case SDL_WINDOWEVENT_SIZE_CHANGED: 
					reshape( e.window.data1, e.window.data2);
					return true;
				default:
					return handleEvent(e);
			}
		case SDL_QUIT:
			return false;
		default:
			return handleEvent(e);
	}
}

int Scene::run(int argc, char **argv)
{
	int retcode;
	glutInit(&argc, argv);
	
	retcode = initWindow();
	if(retcode != 0)
		return retcode;
	initAssimpLog();
		
	try {
		init();
		checkOpenGLError("Scene init");
		int w, h;
		SDL_GetWindowSize(win, &w, &h);
		reshape(w, h);
		checkOpenGLError("Initial Scene reshape");
	
		while (pollEvent() == true){
			unsigned int windowFlags = SDL_GetWindowFlags(win);
			if(windowFlags & (SDL_WINDOW_HIDDEN | SDL_WINDOW_MINIMIZED)){
				continue;
			}

			++nFrames;
			Uint32 start = SDL_GetTicks();
			render();
			checkOpenGLError("Scene render");
			SDL_GL_SwapWindow(win);
			frameTime = (SDL_GetTicks() - start) / 1000.0f;
		}

		clean();
		checkOpenGLError("Scene clean");
		cleanAssimpLog();
		retcode = 0;
	} catch(exception& ex)
	{
		logError(ex.what());
		retcode = 1;
	}

	SDL_DestroyWindow(win);
	SDL_Quit();
	return retcode;
}

SDL_Window *Scene::getWindow() const
{
	return win;
}

SDL_Renderer *Scene::getRenderer() const
{
	return ren;
}

SDL_GLContext Scene::getGlContext() const
{
	return glContext;
}

float Scene::getFrameTime() const
{
	return frameTime;
}

unsigned int Scene::getRenderedFrames() const
{
	return nFrames;
}

void Scene::checkOpenGLError(const string message)
{
	GLenum error = glGetError();
	if(error != GL_NO_ERROR)
		throw SceneError::fromGLError(error, message);
}

void Scene::log(const string& message)
{
	ofstream logStream;
	logStream.open(logFileName(), ios_base::app);
	if (logStream.fail())
		return; // don't throw an exception if log couldn't be opened

	// current date/time based on current system
	time_t now = time(0);
	// convert now to string form
	char *buffer = new char[256];
	ctime_s(buffer, 256, &now);
	string dateString = string(buffer);
	dateString.pop_back();
	delete buffer;

	logStream << dateString << ": " << message << endl;
	logStream.close();
}

string Scene::appIconPath()
{
	return "icon.png";
}