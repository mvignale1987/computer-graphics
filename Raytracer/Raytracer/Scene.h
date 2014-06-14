#pragma once
#include <string>
#include <SDL.h>
#include <pugixml.hpp>
#include "Vector3.h"

class SceneObject;

class Scene 
{
private:
	Vector3 bgColor;
public:
	Scene();
	static Scene readFromPath(const std::string &path);
	
	Vector3 backgroundColor();
private:
	static Vector3 readBackgroundColor(const pugi::xml_document &doc);
};