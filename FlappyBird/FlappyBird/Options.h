#pragma once

#include <SDL_opengl.h>

enum GameSpeed { SLOW = 2, MEDIUM = 0, FAST = 1 };

class Options
{
private:
	bool		textures;
	bool		smoothShading;
	bool		wireframe;
    GameSpeed	gameSpeed;
public:
	Options();

	bool		getTexturesEnabled() const;
	void		setTexturesEnabled(bool val);
	bool		getSmoothShading() const;
	void		setSmoothShading(bool val);
	bool		getWireframeEnabled() const;
	void		setWireframeEnabled(bool val);
	GameSpeed	getGameSpeed() const;
	void		setGameSpeed(GameSpeed val);
};

void glToggle(GLenum capability, GLboolean val);