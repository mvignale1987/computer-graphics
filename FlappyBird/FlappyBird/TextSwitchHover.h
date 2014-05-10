#pragma once

#include "TextHover.h"

class TextSwitchHover : public SceneObject
{
private:
	std::vector<TextHover> texts;
	Mix_Chunk *clickSound;
	int currentIndex;
	bool isClicked;
	bool indexChanged;
public: 
	TextSwitchHover();
	TextSwitchHover(TextOptions normalStyle, TextOptions hoverStyle, Mix_Chunk *hoverSound, ...);

	int getCurrentIndex() const;
	bool wasIndexChanged(); // returns true if index changed since the last time it was called

	void render(Scene &parent);
};