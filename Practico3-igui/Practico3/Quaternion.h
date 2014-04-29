// Adapted from https://github.com/MegaManSE/willperone/blob/master/Math/quaternion.h

/****************************************
* Quaternion class
* By Will Perone (will.perone@gmail.com)
* Original: 12-09-2003
* Revised: 27-09-2003
* 22-11-2003
* 10-12-2003
* 15-01-2004
* 16-04-2004
* 07-29-2011 added corrections from website
* 22-12-2011 added correction to *= operator, thanks Steve Rogers
* 22-10-2012 fixed ctor from euler angles & added non windows platform fixes, thanks to Art Golf
*
* Dependancies: My 4x4 matrix class
*
* © 2003, This code is provided "as is" and you can use it freely as long as
* credit is given to Will Perone in the application it is used in
*
* Notes:
* if |q|=1 then q is a unit Quaternion
* if q=(0,v) then q is a pure Quaternion
* if |q|=1 then q conjugate = q inverse
* if |q|=1 then q= [cos(angle), u*sin(angle)] where u is a unit vector
* q and -q represent the same rotation
* q*q.conjugate = (q.length_squared, 0)
* ln(cos(theta),sin(theta)*v)= ln(e^(theta*v))= (0, theta*v)
****************************************/

#pragma once
 
#include <assert.h>
#include "Vector3.h"


struct Quaternion
{
	float s; //!< the real component
	Vector3 v; //!< the imaginary components

	//! ctors
	Quaternion() {}
	Quaternion(float real, float x, float y, float z): s(real), v(x,y,z) {}
	Quaternion(float real, const Vector3 &i): s(real), v(i) {}

	//! from 3 euler angles
	Quaternion(float theta_z, float theta_y, float theta_x)//float heading, float attitude, float bank)
	{
		float cos_z_2 = cosf(0.5f*theta_z);
		float cos_y_2 = cosf(0.5f*theta_y);
		float cos_x_2 = cosf(0.5f*theta_x);

		float sin_z_2 = sinf(0.5f*theta_z);
		float sin_y_2 = sinf(0.5f*theta_y);
		float sin_x_2 = sinf(0.5f*theta_x);

		// and now compute Quaternion
		s = cos_z_2*cos_y_2*cos_x_2 + sin_z_2*sin_y_2*sin_x_2;
		v = Vector3(
			cos_z_2*cos_y_2*sin_x_2 - sin_z_2*sin_y_2*cos_x_2,
			cos_z_2*sin_y_2*cos_x_2 + sin_z_2*cos_y_2*sin_x_2,
			sin_z_2*cos_y_2*cos_x_2 - cos_z_2*sin_y_2*sin_x_2
		);
	}

	//! from 3 euler angles
	Quaternion(const Vector3 &angles)
	{	
		float cos_z_2 = cosf(0.5f*angles.z());
		float cos_y_2 = cosf(0.5f*angles.y());
		float cos_x_2 = cosf(0.5f*angles.x());

		float sin_z_2 = sinf(0.5f*angles.z());
		float sin_y_2 = sinf(0.5f*angles.y());
		float sin_x_2 = sinf(0.5f*angles.x());

		// and now compute Quaternion
		s = cos_z_2*cos_y_2*cos_x_2 + sin_z_2*sin_y_2*sin_x_2;
		v = Vector3(
			cos_z_2*cos_y_2*sin_x_2 - sin_z_2*sin_y_2*cos_x_2,
			cos_z_2*sin_y_2*cos_x_2 + sin_z_2*cos_y_2*sin_x_2,
			sin_z_2*cos_y_2*cos_x_2 - cos_z_2*sin_y_2*sin_x_2
		);
	}

	//! basic operations
	Quaternion &operator =(const Quaternion &q)	
	{	s= q.s; v= q.v;	return *this;	}

	const Quaternion operator +(const Quaternion &q) const	
	{	return Quaternion(s+q.s, v+q.v);	}

	const Quaternion operator -(const Quaternion &q) const	
	{	return Quaternion(s-q.s, v-q.v);	}

	const Quaternion operator *(const Quaternion &q) const	
	{	return Quaternion(
		s*q.s - v*q.v,
		v.y()*q.v.z() - v.z()*q.v.y() + s*q.v.x() + v.x()*q.s,
		v.z()*q.v.x() - v.x()*q.v.z() + s*q.v.y() + v.y()*q.s,
		v.x()*q.v.y() - v.y()*q.v.x() + s*q.v.z() + v.z()*q.s);
	}

	const Quaternion operator /(const Quaternion &q) const	
	{	
		Quaternion p(q);
		p.invert();
		return *this * p;
	}

	const Quaternion operator *(float scale) const	
	{	return Quaternion(s*scale,v*scale);	}

	const Quaternion operator /(float scale) const
	{	return Quaternion(s/scale,v/scale);	}

	const Quaternion operator -() const	
	{	return Quaternion(-s, -v);	}

	const Quaternion &operator +=(const Quaternion &q)	
	{	v+=q.v; s+=q.s; return *this;	}

	const Quaternion &operator -=(const Quaternion &q)	
	{	v-=q.v; s-=q.s; return *this;	}

	const Quaternion &operator *=(const Quaternion &q)	
	{	
		float x= v.x(), y= v.y(), z= v.z(), sn= s*q.s - v*q.v;
		v = Vector3(
			y*q.v.z() - z*q.v.y() + s*q.v.x() + x*q.s,
			z*q.v.x() - x*q.v.z() + s*q.v.y() + y*q.s,
			x*q.v.y() - y*q.v.x() + s*q.v.z() + z*q.s
		);
		s= sn;
		return *this;
	}

	const Quaternion &operator *= (float scale)	
	{	v*=scale; s*=scale; return *this;	}

	const Quaternion &operator /= (float scale)	
	{	v/=scale; s/=scale; return *this;	}


	//! gets the length of this Quaternion
	float length() const
	{	return (float)sqrt(s*s + v*v); }

	//! gets the squared length of this Quaternion
	float length_squared() const
	{	return (float)(s*s + v*v); }

	//! normalizes this Quaternion
	void normalize()
	{	*this/=length();	}

	//! returns the normalized version of this Quaternion
	Quaternion normalized() const
	{ return *this/length(); }

	//! computes the conjugate of this Quaternion
	void conjugate()
	{	v=-v; }

	//! inverts this Quaternion
	void invert()
	{	conjugate(); *this/=length_squared(); }

	//! returns the logarithm of a Quaternion = v*a where q = [cos(a),v*sin(a)]
	Quaternion log() const
	{
		float a = (float)acos(s);
		float sina = (float)sin(a);
		Quaternion ret;

		ret.s = 0;
		if (sina > 0)
		{
			ret.v = Vector3(
				a*v.x()/sina,
				a*v.y()/sina,
				a*v.z()/sina
			);
		} else {
			ret.v = Vector3::zero;
		}
		return ret;
	}

	//! returns e^Quaternion = exp(v*a) = [cos(a),vsin(a)]
	Quaternion exp() const
	{
		float a = (float)v.length();
		float sina = (float)sin(a);
		float cosa = (float)cos(a);
		Quaternion ret;

		ret.s = cosa;
		if (a > 0)
		{
			ret.v = Vector3(
				sina * v.x() / a,
				sina * v.y() / a,
				sina * v.z() / a
			);
		} else {
			ret.v = Vector3::zero;
		}
		return ret;
	}


	//! computes the dot product of 2 quaternions
	static inline float dot(const Quaternion &q1, const Quaternion &q2)
	{ return q1.v*q2.v + q1.s*q2.s; }

	//! linear Quaternion interpolation
	static Quaternion lerp(const Quaternion &q1, const Quaternion &q2, float t)
	{	return (q1*(1-t) + q2*t).normalized();	}

	//! spherical linear interpolation
	static Quaternion slerp(const Quaternion &q1, const Quaternion &q2, float t)
	{
		Quaternion q3;
		float dot = Quaternion::dot(q1, q2);

		/* dot = cos(theta)
		if (dot < 0), q1 and q2 are more than 90 degrees apart,
		so we can invert one to reduce spinning */
		if (dot < 0)
		{
			dot = -dot;
			q3 = -q2;
		} else q3 = q2;

		if (dot < 0.95f)
		{
			float angle = acosf(dot);
			return (q1*sinf(angle*(1-t)) + q3*sinf(angle*t))/sinf(angle);
		} else // if the angle is small, use linear interpolation
			return lerp(q1,q3,t);	
	}

	//! This version of slerp, used by squad, does not check for theta > 90.
	static Quaternion slerpNoInvert(const Quaternion &q1, const Quaternion &q2, float t)
	{
		float dot = Quaternion::dot(q1, q2);

		if (dot > -0.95f && dot < 0.95f)
		{
			float angle = acosf(dot);	
			return (q1*sinf(angle*(1-t)) + q2*sinf(angle*t))/sinf(angle);
		} else // if the angle is small, use linear interpolation
			return lerp(q1,q2,t);	
	}

	//! spherical cubic interpolation
	static Quaternion squad(const Quaternion &q1,const Quaternion &q2,const Quaternion &a,const Quaternion &b,float t)
	{
		Quaternion c= slerpNoInvert(q1,q2,t),
			d= slerpNoInvert(a,b,t);	
		return slerpNoInvert(c,d,2*t*(1-t));
	}

	//! calculate inner quad point for spherical cubic interpolation 2
	static Quaternion innerQuadPoint(const Quaternion &q0,const Quaternion &q1,const Quaternion &q2)
	{
		Quaternion vTemp, vTemp1, vTemp2;
		Quaternion qResult;

		vTemp = q1;
		vTemp.invert();

		vTemp1 = (vTemp * q0);
		vTemp1 = vTemp1.log();

		vTemp2 = (vTemp * q2);
		vTemp2 = vTemp2.log();

		vTemp = (vTemp1 + vTemp2) * (-0.25f);

		qResult = q1 * vTemp.exp();

		return qResult;
	}

	//! spherical cubic interpolation alternate approach
	//! this method is smoother than the original squad but only works when the angles are less than 90 degrees apart
	static Quaternion squad2(const Quaternion &q1,const Quaternion &q2,const Quaternion &q3,const Quaternion &q4,float t)
	{
		Quaternion a= innerQuadPoint(q1,q2,q3),
			b= innerQuadPoint(q2,q3,q4);
		return slerpNoInvert(slerpNoInvert(q2,q3,t),slerpNoInvert(a,b,t),2.0f*t*(1.0f-t));
	}

	//! Shoemake-Bezier interpolation using De Castlejau algorithm
	static Quaternion bezier(const Quaternion &q1,const Quaternion &q2,const Quaternion &a,const Quaternion &b,float t)
	{
		// level 1
		Quaternion q11= slerpNoInvert(q1,a,t),
			q12= slerpNoInvert(a,b,t),
			q13= slerpNoInvert(b,q2,t);	
		// level 2 and 3
		return slerpNoInvert(slerpNoInvert(q11,q12,t), slerpNoInvert(q12,q13,t), t);
	}

	//! Given 3 quaternions, qn-1,qn and qn+1, calculate a control point to be used in spline interpolation
	static Quaternion spline(const Quaternion &qnm1,const Quaternion &qn,const Quaternion &qnp1)
	{
		Quaternion qni(qn.s, -qn.v);	

		return qn * (( (qni*qnm1).log()+(qni*qnp1).log() )/-4).exp();
	}

	//! converts from a normalized axis - angle pair rotation to a Quaternion
	static inline Quaternion from_axis_angle(const Vector3 &axis, float angle)
	{	return Quaternion(cosf(angle/2), axis*sinf(angle/2)); }

	//! returns the axis and angle of this unit Quaternion
	void toAxisAngle(Vector3 &axis, float &angle) const
	{
		angle = acosf(s);

		// pre-compute to save time
		float sinf_theta_inv = 1.0f/sinf(angle);

		// now the vector
		axis = Vector3(
			v.x() * sinf_theta_inv,
			v.y() * sinf_theta_inv,
			v.z() * sinf_theta_inv
		);

		// multiply by 2
		angle*=2;
	}

	//! rotates v by this Quaternion (Quaternion must be unit)
	Vector3 rotate(const Vector3 &v)
	{
		Quaternion V(0, v);
		Quaternion conjugate(*this);
		conjugate.conjugate();
		return (*this * V * conjugate).v;
	}

	//! returns the euler angles from a rotation Quaternion
	Vector3 eulerAngles(bool homogenous=true) const
	{
		float sqw = s*s;
		float sqx = v.x()*v.x();
		float sqy = v.y()*v.y();
		float sqz = v.z()*v.z();

		if (homogenous) {
			return Vector3(
				atan2f(2.f * (v.x()*v.y() + v.z()*s), sqx - sqy - sqz + sqw),
				asinf(-2.f * (v.x()*v.z() - v.y()*s)),
				atan2f(2.f * (v.y()*v.z() + v.x()*s), -sqx - sqy + sqz + sqw)
				);
		} else {
			return Vector3(
				atan2f(2.f * (v.z()*v.y() + v.x()*s), 1 - 2*(sqx + sqy)),
				asinf(-2.f * (v.x()*v.z() - v.y()*s)),
				atan2f(2.f * (v.x()*v.y() + v.z()*s), 1 - 2*(sqy + sqz))
			);
		}
	}
};


void glRotate(const Quaternion& q)
{
	Vector3 axis;
	float angle;
	q.normalized().toAxisAngle(axis, angle);
	angle = angle * 180 / (GLfloat) M_PI;
	glRotatef(angle, axis.x(), axis.y(), axis.z());
}

