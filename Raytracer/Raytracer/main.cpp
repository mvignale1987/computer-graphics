#include <iostream>
#include <stdexcept>
#include "Scene.h"
#include "OpenGLRenderer.h"
#include "App.h"

using namespace std;

void printUsage(const char *programName)
{
	cerr << "Error: missing scene parameter" << endl
		<< "\t" << "Usage: " << programName <<  " <escena>" << endl;
}

int main(int argc, char **argv)
{
	if(argc == 1){
		printUsage(argv[0]);
		return 1;
	}

	Scene scene;
	try {
		scene = Scene::readFromPath(argv[1]);
	} catch(const exception &ex) {
		cerr << "Error Reading Scene: " << ex.what() << endl
			<< "Terminating!" << endl;
		return 1;
	}

	App app;
	OpenGLRenderer renderer(app, scene);
	app.setRenderer(&renderer);

	return app.run(argc, argv);
}