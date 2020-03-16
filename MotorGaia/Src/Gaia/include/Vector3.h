#pragma once
#ifndef VECTOR3_H
#define VECTOR3_H

#define CHECK_VALID( _v ) 0
#define Assert( _exp ) ((void)0)

class btVector3;

struct Vector3
{
	double x;
	double y;
	double z;

	Vector3();
	Vector3(double x, double y, double z);


	void normalize();

	bool operator==(const Vector3& v) const;
	bool operator!=(const Vector3& v) const;
	Vector3& operator+=(const Vector3& v);
	Vector3& operator-=(const Vector3& v);
	Vector3& operator*=(const Vector3& v);
	Vector3& operator*=(double n);
	Vector3& operator/=(const Vector3& v);
	Vector3& operator/=(double n);
	Vector3& operator+=(double n);
	Vector3& operator-=(double n);

	Vector3& operator=(const Vector3& v);
	Vector3	operator+(const Vector3& v) const;
	Vector3	operator-(const Vector3& v) const;
	Vector3	operator*(const Vector3& v) const;
	Vector3	operator/(const Vector3& v) const;
	Vector3	operator*(double n) const;
	Vector3	operator/(double n) const;

	double magnitudeSquared();
	double magnitude();
	Vector3& normalized();

	void rotateAroundAxis(const Vector3& axis, double angle);
	void lerp(const Vector3& v, double percentage);
	void lerp(const Vector3& v, const Vector3& percentage);

};

Vector3 rotateAroundPivot(const Vector3& point, const Vector3& pivot, const Vector3& angles);

Vector3 operator +(const Vector3& p1, const btVector3& p2);
btVector3 operator +(const btVector3& p1, const Vector3& p2);

#endif