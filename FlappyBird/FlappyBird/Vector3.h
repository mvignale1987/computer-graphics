#ifndef VECTOR3_H
#define VECTOR3_H

#include <SDL_opengl.h>
#include <iostream>
#include <string>

class Vector3 {
private:
	GLfloat coords[3];
public:
	Vector3(); // Vector3(0, 0, 0)
	Vector3(GLfloat x, GLfloat y, GLfloat z);
	explicit Vector3(const GLfloat*); // from 3 sized vector
	static Vector3 random(); // returns a random vector with x,y,z in [0, 1)

	GLfloat x() const; // x coordinate
	GLfloat y() const; // y coordinate
	GLfloat z() const; // z coordinate
	const GLfloat *vector() const;


	// basic operators
	Vector3 operator -() const;
	Vector3 operator +() const;
	Vector3 operator +(const Vector3&) const;
	Vector3 operator -(const Vector3&) const;
	Vector3 operator *(GLfloat) const;
	Vector3 operator /(GLfloat) const;
	Vector3& operator += (const Vector3 &);
	Vector3& operator -= (const Vector3 &);
	Vector3& operator *= (GLfloat);
	GLfloat operator *(const Vector3& v) const;
	Vector3& operator /=(GLfloat);
	bool operator==(const Vector3&) const;
	bool operator!=(const Vector3&) const;

	std::string toString() const;	// returns "(x,y,z)"

	static const Vector3 up;		// ( 0, 1, 0)
	static const Vector3 down;		// ( 0,-1, 0)
	static const Vector3 left;		// (-1, 0, 0)
	static const Vector3 right;		// ( 1, 0, 0)
	static const Vector3 forward;	// ( 0, 0, 1)
	static const Vector3 backward;  // ( 0, 0,-1)
	static const Vector3 zero;		// ( 0, 0, 0)
	static const Vector3 one;		// ( 1, 1, 1)

	// util Functions
	GLfloat length(void) const;
    GLfloat lengthSquared(void) const;
    Vector3 normalized() const;
    GLfloat dot(const Vector3& v) const;
    Vector3 cross(const Vector3& v) const;
    GLfloat distance(const Vector3& v) const;
    GLfloat distanceSquared(const Vector3& v) const;
	Vector3 scale(GLfloat x, GLfloat y, GLfloat z) const;
};

Vector3 operator * (GLfloat k, const Vector3& v);
void glTranslate(const Vector3& v);
void glRotate(GLfloat angle, const Vector3& v);
void glColor(const Vector3& v);
void glVertex(const Vector3& v);
std::ostream& operator <<(std::ostream& stream, const Vector3& v);

#endif