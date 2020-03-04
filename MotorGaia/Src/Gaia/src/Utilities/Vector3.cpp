#include "Vector3.h"
#include<cmath>

Vector3::Vector3()
{
	x = 0;
	y = 0;
	z = 0;
}

Vector3::Vector3(double x, double y, double z) : x(x), y(y), z(z)
{

}

Vector3 Vector3::operator+=(const Vector3& p)
{
	this->x += p.x;
	this->y += p.y;
	this->z += p.z;
	return *this;
}

Vector3 Vector3::operator-=(const Vector3& p)
{
	this->x -= p.x;
	this->y -= p.y;
	this->z -= p.z;
	return *this;
}

Vector3 Vector3::operator/=(const Vector3& p)
{
	this->x /= p.x;
	this->y /= p.y;
	this->z /= p.z;
	return *this;
}

Vector3 Vector3::operator*=(const Vector3& p)
{
	this->x *= p.x;
	this->y *= p.y;
	this->z *= p.z;
	return *this;
}

Vector3 Vector3::operator/=(const double d)
{
	this->x /= d;
	this->y /= d;
	this->z /= d;
	return *this;
}

Vector3 Vector3::operator*=(const double d)
{
	this->x *= d;
	this->y *= d;
	this->z *= d;
	return *this;
}

bool Vector3::operator == (const Vector3& p) const
{
	return this->x == p.x && this->y == p.y && this->z == p.z;
}

Vector3 Vector3::operator=(const Vector3& p)
{
	//Comprueba que no se este intentando igualar un vector3 a si mismo
	if (this != &p)
	{
		this->x = p.x;
		this->y = p.y;
		this->z = p.z;
	}
	return *this;
}

void Vector3::normalize()
{
	*this /= magnitude();
}

Vector3 Vector3::normalized()
{
	Vector3 aux = *this / magnitude();
	return aux;
}

double Vector3::magnitudeSquared()
{
	return x * x + y * y + z * z;
}

double Vector3::magnitude()
{
	return sqrt(magnitudeSquared());
}

Vector3 operator +(const Vector3& p1, const Vector3& p2)
{
	return Vector3(p1.x + p2.x, p1.y + p2.y, p1.z + p2.z);
}

Vector3 operator -(const Vector3& p1, const Vector3& p2)
{
	return Vector3(p1.x - p2.x, p1.y - p2.y, p1.z - p2.z);
}

Vector3 operator*(const Vector3& p1, const Vector3& p2)
{
	return Vector3(p1.x * p2.x, p1.y * p2.y, p1.z * p2.z);
}

Vector3 operator/(const Vector3& p1, const Vector3& p2)
{
	return Vector3(p1.x / p2.x, p1.y / p2.y, p1.z / p2.z);
}

Vector3 operator*(const Vector3& p1, const double d)
{
	return Vector3(p1.x * d, p1.y * d, p1.z * d);
}

Vector3 operator/(const Vector3& p1, const double d)
{
	return Vector3(p1.x / d, p1.y / d, p1.z / d);
}

