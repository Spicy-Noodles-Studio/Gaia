#include "Vector3.h"

#include<cmath>
#include <btBulletDynamicsCommon.h>
#include "Quaternion.h"

Vector3::Vector3()
{
	x = 0;
	y = 0;
	z = 0;
}

Vector3::Vector3(double x, double y, double z) : x(x), y(y), z(z)
{

}

bool Vector3::operator==(const Vector3& v) const
{
	CHECK_VALID(v);
	CHECK_VALID(*this);
	return (v.x == x) && (v.y == y) && (v.z == z);
}

bool Vector3::operator!=(const Vector3& v) const
{
	CHECK_VALID(src);
	CHECK_VALID(*this);
	return (v.x != x) || (v.y != y) || (v.z != z);
}

Vector3& Vector3::operator+=(const Vector3& v)
{
	CHECK_VALID(*this);
	CHECK_VALID(v);
	x += v.x; y += v.y; z += v.z;
	return *this;
}

Vector3& Vector3::operator-=(const Vector3& v)
{
	CHECK_VALID(*this);
	CHECK_VALID(v);
	x -= v.x; y -= v.y; z -= v.z;
	return *this;
}

Vector3& Vector3::operator*=(double n)
{
	x *= n;
	y *= n;
	z *= n;
	CHECK_VALID(*this);
	return *this;
}

Vector3& Vector3::operator*=(const Vector3& v)
{
	CHECK_VALID(v);
	x *= v.x;
	y *= v.y;
	z *= v.z;
	CHECK_VALID(*this);
	return *this;
}

Vector3& Vector3::operator+=(double n)
{
	x += n;
	y += n;
	z += n;
	CHECK_VALID(*this);
	return *this;
}

Vector3& Vector3::operator-=(double n)
{
	x -= n;
	y -= n;
	z -= n;
	CHECK_VALID(*this);
	return *this;
}

Vector3& Vector3::operator/=(double n)
{
	Assert(n != 0.f);
	double d = 1.0f / n;
	x *= d;
	y *= d;
	z *= d;
	CHECK_VALID(*this);
	return *this;
}

Vector3& Vector3::operator/=(const Vector3& v)
{
	CHECK_VALID(v);
	Assert(v.x != 0.f && v.y != 0.f && v.z != 0.f);
	x /= v.x;
	y /= v.y;
	z /= v.z;
	CHECK_VALID(*this);
	return *this;
}

Vector3& Vector3::operator=(const Vector3& v)
{
	CHECK_VALID(v);
	x = v.x; y = v.y; z = v.z;
	return *this;
}

Vector3 Vector3::operator+(const Vector3& v) const
{
	Vector3 res;
	res.x = x + v.x;
	res.y = y + v.y;
	res.z = z + v.z;
	return res;
}

Vector3 Vector3::operator-(const Vector3& v) const
{
	Vector3 res;
	res.x = x - v.x;
	res.y = y - v.y;
	res.z = z - v.z;
	return res;
}

Vector3 Vector3::operator*(double n) const
{
	Vector3 res;
	res.x = x * n;
	res.y = y * n;
	res.z = z * n;
	return res;
}

Vector3 Vector3::operator*(const Vector3& v) const
{
	Vector3 res;
	res.x = x * v.x;
	res.y = y * v.y;
	res.z = z * v.z;
	return res;
}

Vector3 Vector3::operator/(double n) const
{
	Vector3 res;
	res.x = x / n;
	res.y = y / n;
	res.z = z / n;
	return res;
}

Vector3 Vector3::operator/(const Vector3& v) const
{
	Vector3 res;
	res.x = x / v.x;
	res.y = y / v.y;
	res.z = z / v.z;
	return res;
}

void Vector3::normalize()
{
	*this /= magnitude();
}

Vector3& Vector3::normalized()
{
	Vector3 aux = *this / magnitude();
	return aux;
}

Vector3 rotateAroundPivot(const Vector3& point, const Vector3& pivot, const Vector3& angles)
{
	Vector3 v = { point.x - pivot.x,point.y - pivot.y, point.z - pivot.z };
	Quaternion q = ToQuaternion(angles.z, angles.y, angles.x), q1 = q.inverse(), aux = { 0,v.x,v.y,v.z }, aux1;
	aux1 = hamilton(q, aux);
	aux1 = hamilton(aux1, q1);
	v = { std::round(aux1.x),std::round(aux1.y),std::round(aux1.z) };
	return (v + pivot);
}

void Vector3::rotateAroundAxis(const Vector3& axis, double angle)
{
	Vector3 result = *this;
	double ang = angle * SIMD_RADS_PER_DEG;
	if (axis == Vector3(0, 0, 1)) {
		result.x = x * cos(ang) - y * sin(ang);
		result.y = x * sin(ang) + y * cos(ang);
	}
	else if (axis == Vector3(0, 1, 0)) {
		result.x = x * cos(ang) + z * sin(ang);
		result.z = -x * sin(ang) + z * cos(ang);
	}
	else if (axis == Vector3(1, 0, 0)) {
		result.y = y * cos(ang) - z * sin(ang);
		result.z = y * sin(ang) + z * cos(ang);
	}
	*this = result;
}

// percentage has to be a value between 0 and 1
void Vector3::lerp(const Vector3& v, double percentage)
{
	lerp(v, { percentage,percentage,percentage });
}

void Vector3::lerp(const Vector3& v, const Vector3& percentage)
{
	*this = { x + (v.x - x) * percentage.x, y + (v.y - y) * percentage.y, z + (v.z - z) * percentage.z };
}

double Vector3::magnitudeSquared()
{
	return x * x + y * y + z * z;
}

double Vector3::magnitude()
{
	return sqrt(magnitudeSquared());
}


Vector3 operator+(const Vector3& p1, const btVector3& p2)
{
	return Vector3(p1.x + p2.x(), p1.y + p2.y(), p1.z + p2.z());
}

btVector3 operator+(const btVector3& p1, const Vector3& p2)
{
	return btVector3(p1.x() + btScalar(p2.x), p1.y() + btScalar(p2.y), p1.z() + btScalar(p2.z));
}
