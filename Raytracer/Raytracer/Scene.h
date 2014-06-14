#pragma once
#include <string>
#include <SDL.h>
#include <pugixml.hpp>
#include "Vector3.h"
#include "Camera.h"

class SceneObject;

class Scene 
{
private:
	Vector3 bgColor;
	Camera cam;
public:
	Scene();
	static Scene readFromPath(const std::string &path);
	
	Vector3 backgroundColor();
private:
	static Vector3 readBackgroundColor(const pugi::xml_node &scene);
	static Camera  readCamera(const pugi::xml_node &scene);
	
	static Vector3 vectorFromChild(const pugi::xml_node &node, const std::string &child);
	static float  floatFromChild(const pugi::xml_node &node, const std::string &child);
};