#pragma once
#ifndef VECTOR3_H
#define VECTOR3_H

#include "GaiaExports.h"

class btVector3;

class GAIA_API Vector3
{
public:
	double x;
	double y;
	double z;

	Vector3();
	Vector3(double x, double y, double z);


	void normalize();

	Vector3 operator +=(const Vector3 p);
	Vector3 operator -=(const Vector3 p);
	bool operator == (const Vector3 p) const;
	Vector3 operator = (const Vector3 p);

	Vector3& operator /= (const Vector3& p);
	Vector3& operator *= (const Vector3& p);
	Vector3& operator /= (const double d);
	Vector3& operator *= (const double d);

	double magnitudeSquared();
	double magnitude();
	Vector3& normalized();

	void rotateAroundAxis(const Vector3& axis, double angle);
	void lerp(const Vector3& v, double percentage);
	void lerp(const Vector3& v, const Vector3& percentage);
	Vector3 rotateAroundPivot(const Vector3& point, const Vector3& pivot, const Vector3& angles);

	Vector3 operator +(const Vector3& p2) const;
	Vector3 operator +(const btVector3& p2) const;
	Vector3 operator -(const Vector3& p2) const;
	Vector3 operator *(const Vector3& p2) const;
	Vector3 operator /(const Vector3& p2) const;
	Vector3 operator *(const double d) const;
	Vector3 operator /(const double d) const;

};

btVector3 operator +(const btVector3& p1, const Vector3& p2);

#endif