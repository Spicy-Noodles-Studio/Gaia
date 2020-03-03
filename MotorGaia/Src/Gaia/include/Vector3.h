#pragma once
#ifndef VECTOR3_H
#define VECTOR3_H

struct Vector3
{
	double x;
	double y;
	double z;

	Vector3();
	Vector3(double x, double y, double z);

	Vector3 operator +=(const Vector3& p);
	Vector3 operator -=(const Vector3& p);
	bool operator == (const Vector3& p) const;
	Vector3 operator = (const Vector3& p);
};

Vector3 operator +(const Vector3& p1, const Vector3& p2);
Vector3 operator -(const Vector3& p1, const Vector3& p2);

#endif