#include "Vector2.h"
#include <GL\GLU.h>
#include <sstream>

const Vector2 Vector2::down     = Vector2( 0, -1);
const Vector2 Vector2::up       = Vector2( 0,  1);
const Vector2 Vector2::left     = Vector2(-1,  0);
const Vector2 Vector2::right    = Vector2( 1,  0);
const Vector2 Vector2::zero     = Vector2( 0,  0);
const Vector2 Vector2::one      = Vector2( 1,  1);

Vector2::Vector2()
{
	coords[0] = coords[1] = 0;
}

Vector2::Vector2(GLfloat x, GLfloat y)
{
	coords[0] = x;
	coords[1] = y;
}

Vector2 Vector2::random()
{
	return Vector2(rand() / (GLfloat)RAND_MAX, rand() / (GLfloat)RAND_MAX);
}

GLfloat Vector2::x() const
{
	return coords[0];
}

GLfloat Vector2::y() const
{
	return coords[1];
}

const GLfloat *Vector2::vector() const
{
	return coords;
}

bool Vector2::operator==(const Vector2& v) const
{
	return ((coords[0] == v.coords[0]) &&
		(coords[1] == v.coords[1]));
}


bool Vector2::operator!=(const Vector2& v) const
{
	return ((coords[0] != v.coords[0]) ||
		(coords[1] != v.coords[1]));
}


GLfloat Vector2::length(void) const
{
	return sqrt(lengthSquared());
}


GLfloat Vector2::lengthSquared(void) const
{
	return coords[0]*coords[0] + coords[1]*coords[1];
}


Vector2 Vector2::normalized() const
{
	return Vector2(*this) / length();
}


GLfloat Vector2::dot(const Vector2& v) const
{
	return coords[0]*v.coords[0] + coords[1]*v.coords[1];
}


GLfloat Vector2::distance(const Vector2& v) const
{
	return sqrt(distanceSquared(v));
}


GLfloat Vector2::distanceSquared(const Vector2& v) const
{
	float dx, dy;
	dx = coords[0] - v.coords[0];
	dy = coords[1] - v.coords[1];
	return dx*dx + dy*dy;
}

Vector2 Vector2::scale(GLfloat x, GLfloat y) const 
{
	return Vector2(coords[0] * x, coords[1] * y);
}

Vector2 Vector2::clamped() const
{
	float x = coords[0];
	float y = coords[1];
	if(x < 0)
		x = 0;
	else if(x > 1)
		x = 1;
	if(y < 0)
		y = 0;
	else if(y > 1)
		y = 1;
	return Vector2(x,y);
}

Vector2 Vector2::operator -() const
{
	return Vector2(-coords[0], -coords[1]);
}
Vector2 Vector2::operator +() const
{
	return Vector2(coords[0], coords[1]);
}

Vector2 Vector2::operator +(const Vector2&v) const
{
	return Vector2(coords[0] + v.coords[0], coords[1]+v.coords[1]);
}

Vector2& Vector2::operator +=(const Vector2&v)
{
	coords[0] += v.coords[0];
	coords[1] += v.coords[1];
	return *this;
}

Vector2& Vector2::operator -=(const Vector2&v)
{
	coords[0] -= v.coords[0];
	coords[1] -= v.coords[1];
	return *this;
}

Vector2& Vector2::operator *=(GLfloat k)
{
	coords[0] *= k;
	coords[1] *= k;
	return *this;
}

Vector2 Vector2::operator -(const Vector2&v) const
{
	return Vector2(coords[0] - v.coords[0], coords[1] - v.coords[1]);
}

Vector2 Vector2::operator *(GLfloat k) const
{
	return Vector2(coords[0] * k, coords[1] * k);
}

GLfloat Vector2::operator *(const Vector2& v) const
{
	return v.dot(*this);
}

Vector2 Vector2::operator /(GLfloat k) const
{
	return Vector2(coords[0] / k, coords[1] / k);
}

Vector2& Vector2::operator /=(GLfloat k)
{
	coords[0] /= k;
	coords[1] /= k;
	return *this;
}

Vector2 operator * (GLfloat k, const Vector2& v)
{
	return v * k;
}


std::string Vector2::toString() const
{
	std::stringstream ss;
	ss << "(" << coords[0] << ", " << coords[1] << ")";
	return ss.str();
}

std::ostream& operator <<(std::ostream& s, const Vector2& v)
{
	return s << v.toString();
}
