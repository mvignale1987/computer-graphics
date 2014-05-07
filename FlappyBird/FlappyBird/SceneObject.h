#pragma once

#include "Scene.h"

class SceneObject 
{
public:
	virtual void render(Scene &parent);
	virtual void clean(Scene &parent);
};