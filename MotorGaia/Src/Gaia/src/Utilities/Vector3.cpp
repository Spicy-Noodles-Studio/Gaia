#include "Vector3.h"

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