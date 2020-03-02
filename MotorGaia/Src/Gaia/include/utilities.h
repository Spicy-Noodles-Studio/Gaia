#pragma once
#ifndef  UTILITIES_H
#define UTILITIES_H

enum Axis { X, Y, Z };

enum SpaceReference { LocalSpace, WorldSpace, ParentSpace };

enum LightType{Directional,Point,Spotlight};

struct Vector3 {
	double x;
	double y;
	double z;

	Vector3 operator +=(const Vector3& p);
	Vector3 operator -=(const Vector3& p);
	bool operator == (const Vector3& p) const;
	Vector3 operator = (const Vector3& p);
};

Vector3 operator +(const Vector3& p1, const Vector3& p2);

Vector3 operator -(const Vector3& p1, const Vector3& p2);

#endif