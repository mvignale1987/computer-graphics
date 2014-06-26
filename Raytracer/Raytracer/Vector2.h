#pragma once

#include <SDL_opengl.h>
#include <SDL.h>
#include <iostream>
#include <string>

class Vector2 {
private:
	GLfloat coords[2];
public:
	Vector2(); // Vector2(0, 0)
	Vector2(GLfloat x, GLfloat y);
	explicit Vector2(const GLfloat*); // from 2 sized vector
	static Vector2 random(); // returns a random vector with x,y in [0, 1)

	GLfloat x() const; // x coordinate
	GLfloat y() const; // y coordinate
	const GLfloat *vector() const;


	// basic operators
	Vector2 operator -() const;
	Vector2 operator +() const;
	Vector2 operator +(const Vector2&) const;
	Vector2 operator -(const Vector2&) const;
	Vector2 operator *(GLfloat) const;
	Vector2 operator /(GLfloat) const;
	Vector2& operator += (const Vector2 &);
	Vector2& operator -= (const Vector2 &);
	Vector2& operator *= (GLfloat);
	GLfloat operator *(const Vector2& v) const;
	Vector2& operator /=(GLfloat);
	bool operator==(const Vector2&) const;
	bool operator!=(const Vector2&) const;

	std::string toString() const;	// returns "(x,y)"

	static const Vector2 up;		// ( 0, 1)
	static const Vector2 down;		// ( 0,-1)
	static const Vector2 left;		// (-1, 0)
	static const Vector2 right;		// ( 1, 0)
	static const Vector2 zero;		// ( 0, 0)
	static const Vector2 one;		// ( 1, 1)

	// util Functions
	GLfloat length(void) const;
    GLfloat lengthSquared(void) const;
    Vector2 normalized() const;
    GLfloat dot(const Vector2& v) const;
    GLfloat distance(const Vector2& v) const;
    GLfloat distanceSquared(const Vector2& v) const;
	Vector2 scale(GLfloat x, GLfloat y) const;

	Vector2 clamped() const;
};

Vector2 operator * (GLfloat k, const Vector2& v);
std::ostream& operator <<(std::ostream& stream, const Vector2& v);