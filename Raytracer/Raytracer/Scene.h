#pragma once
#include <string>
#include <vector>
#include <SDL.h>

class SceneObject;

class Scene 
{
public:
	Scene();
	static Scene readFromPath(const std::string &path);
	std::vector<SceneObject *> getObjects() const;
};