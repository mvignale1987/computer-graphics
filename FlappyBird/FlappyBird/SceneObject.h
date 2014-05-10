#pragma once

#include "Scene.h"

// Interface que cumplen los objetos que son hijos de un Scene
class SceneObject 
{
protected: // para no permitir crear SceneObjects explicitamente
	SceneObject();
public:
	virtual void render(Scene &parent);
	virtual void handleEvent(Scene &parent, const SDL_Event& ev);
	virtual void clean(Scene &parent);
	virtual ~SceneObject();
};