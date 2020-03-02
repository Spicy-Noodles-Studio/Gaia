#include "utilities.h"

Vector3 operator +(const Vector3& p1, const Vector3& p2)
{
	Vector3 result;
	result.x = p1.x + p2.x;
	result.y = p1.y + p2.y;
	result.z = p1.z + p2.z;
	return result;
}
Vector3 operator -(const Vector3& p1, const Vector3& p2)
{
	Vector3 result;
	result.x = p1.x - p2.x;
	result.y = p1.y - p2.y;
	result.z = p1.z - p2.z;
	return result;
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

bool Vector3::operator == (const Vector3& p) const
{
	return this->x == p.x && this->y == p.y && this->z == p.z;
}

Vector3 Vector3::operator=(const Vector3& p)
{
	if (this != &p) { //Comprueba que no se este intentando igualar un vector3 a si mismo
		this->x = p.x;
		this->y = p.y;
		this->z = p.z;
	}
	return *this;
}
