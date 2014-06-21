#include "Logger.h"
#include <fstream>
#include <iostream>
#include <Windows.h>
#include <SDL.h>
#include <sstream>
#include <iomanip>

using namespace std;

//const std::string Logger::logFileName = "log.txt";
const std::string Logger::logFileName = "";

static void logFile(const string& message)
{
	ofstream logStream;
	logStream.open(Logger::logFileName, ios_base::app);
	if (logStream.fail())
		return; // don't throw an exception if log couldn't be opened

	logStream << message;
	logStream.flush();
	logStream.close();
}

static void logStdout(const string& message)
{
	cout << message;
	cout.flush();
}

void Logger::log(const string& message)
{
	stringstream ss;
	ss << std::fixed << setprecision(3) << SDL_GetTicks() / 1000.0 << ": " << message  << endl;
	(logFileName.empty() ? logStdout : logFile)(ss.str());
}


void Logger::logSDLError(const string &prefix)
{
	string msg = prefix + ": " + SDL_GetError();
	log(msg);
}

void Logger::logError(const string &msg)
{
	MessageBox(NULL, msg.c_str(), "Error", MB_OK | MB_ICONERROR);
}
