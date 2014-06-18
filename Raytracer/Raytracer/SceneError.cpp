#include "SceneError.h"

#include <sstream>
#include <SDL.h>
#include <SDL_opengl.h>
#include <cstring>

SceneError::SceneError(const char *msg):
	exception(msg)
{
}

SceneError fromString(const string &s)
{
	char *message = new char[s.length() + 1];
	strcpy_s(message, s.length()+1, s.c_str());
	return SceneError(message);
}

SceneError SceneError::fromSDLError(const string &msg)
{
	std::stringstream ss;
	ss << msg << ": " << SDL_GetError();
	return fromString(ss.str());
}

SceneError SceneError::fromGLError(GLenum errorEnum, const string &prefix)
{
	string message;

	switch(errorEnum)
	{
		case GL_INVALID_ENUM:
			message = "OpenGL GL_INVALID_ENUM: An unacceptable value was specified for an enumerated argument";
			break;
		case GL_INVALID_VALUE:
			message = "OpenGL GL_INVALID_VALUE: A numeric argument was out of range";
			break;
		case GL_INVALID_OPERATION:
			message = "OpenGL GL_INVALID_OPERATION: An operation was not allowed in the current state";
			break;
		case GL_INVALID_FRAMEBUFFER_OPERATION:
			message = "OpenGL GL_INVALID_FRAMEBUFFER_OPERATION: The framebuffer object is not complete";
		case GL_STACK_OVERFLOW:
			message = "OpenGL GL_STACK_OVERFLOW: An attempt has been made to perform an operation that would cause an internal stack to overflow"; 
			break;
		case GL_STACK_UNDERFLOW:
			message = "OpenGL GL_STACK_OVERFLOW: An attempt has been made to perform an operation that would cause an internal stack to underflow";
			break;
		case GL_OUT_OF_MEMORY:
			message = "OpenGL GL_OUT_OF_MEMORY: There is not enough memory left to execute the command";
			break;
		case GL_NO_ERROR:
			message = "OpenGL GL_NO_ERROR: No error recorded";
			break;
		default:
			message = "OpenGL Unknown error";
			break;
	}
	std::stringstream ss;
	ss << prefix << ": " << message;
	return fromString(ss.str());
}

SceneError SceneError::fromILError(ILenum errorEnum, const string &prefix)
{
	string message;

	switch(errorEnum)
	{
		case IL_NO_ERROR:
			message = "DevIL IL_NO_ERROR";
			break;
		case IL_INVALID_ENUM:
			message = "DevIL IL_INVALID_ENUM";
			break;
		case IL_OUT_OF_MEMORY:
			message = "DevIL IL_OUT_OF_MEMORY";
			break;
		case IL_FORMAT_NOT_SUPPORTED:
			message = "DevIL IL_FORMAT_NOT_SUPPORTED";
			break;
		case IL_INTERNAL_ERROR:
			message = "DevIL IL_INTERNAL_ERROR";
			break;
		case IL_INVALID_VALUE:
			message = "DevIL IL_INVALID_VALUE";
			break;
		case IL_ILLEGAL_OPERATION:
			message = "DevIL IL_ILLEGAL_OPERATION";
			break;
		case IL_ILLEGAL_FILE_VALUE:
			message = "DevIL IL_ILLEGAL_FILE_VALUE";
			break;
		case IL_INVALID_FILE_HEADER:
			message = "DevIL IL_INVALID_FILE_HEADER";
			break;
		case IL_INVALID_PARAM:
			message = "DevIL IL_INVALID_PARAM";
			break;
		case IL_COULD_NOT_OPEN_FILE:
			message = "DevIL IL_COULD_NOT_OPEN_FILE";
			break;
		case IL_INVALID_EXTENSION:
			message = "DevIL IL_INVALID_EXTENSION";
			break;
		case IL_FILE_ALREADY_EXISTS:
			message = "DevIL IL_FILE_ALREADY_EXISTS";
			break;
		case IL_OUT_FORMAT_SAME:
			message = "DevIL IL_OUT_FORMAT_SAME";
			break;
		case IL_STACK_OVERFLOW:
			message = "DevIL IL_STACK_OVERFLOW";
			break;
		case IL_STACK_UNDERFLOW:
			message = "DevIL IL_STACK_UNDERFLOW";
			break;
		case IL_INVALID_CONVERSION:
			message = "DevIL IL_INVALID_CONVERSION";
			break;
		case IL_LIB_GIF_ERROR:
			message = "DevIL IL_LIB_GIF_ERROR";
			break;
		case IL_LIB_JPEG_ERROR:
			message = "DevIL IL_LIB_JPEG_ERROR";
			break;
		case IL_LIB_PNG_ERROR:
			message = "DevIL IL_LIB_PNG_ERROR";
			break;
		case IL_LIB_TIFF_ERROR:
			message = "DevIL IL_LIB_TIFF_ERROR";
			break;
		case IL_LIB_MNG_ERROR:
			message = "DevIL IL_LIB_MNG_ERROR";
			break;
		case IL_UNKNOWN_ERROR:
			message = "DevIL IL_UNKNOWN_ERROR";
			break;
		default:
			message = "IL Unknown error";
			break;
	}
	std::stringstream ss;
	ss << prefix << ": " << message;
	return fromString(ss.str());
}

SceneError::~SceneError()
{
}