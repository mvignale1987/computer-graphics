#pragma once

class Rect
{
private:
	float borders[4]; // top, left, bottom, right
public:
	Rect();
	Rect(float top, float left, float bottom, float right);
	Rect(int top, int left, int bottom, int right);

	// getters
	float top() const;
	float left() const;
	float bottom() const;
	float right() const;

	// utils
	bool contains(float x, float y) const;
	bool contains(int x, int y) const;
};
