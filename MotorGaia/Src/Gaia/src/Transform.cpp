#include "Transform.h"

Transform::Transform(GameObject* gameObject) : GaiaComponent(gameObject)
{
	position.x = 0;
	position.y = 0;
	position.z = 0;

	scale.x = 1;
	scale.y = 1;
	scale.z = 1;

	rotation.x = 0;
	rotation.y = 0;
	rotation.z = 0;
}

Transform::~Transform()
{

}

void Transform::setPosition(double xPos, double yPos, double zPos)
{
	position.x = xPos;
	position.y = yPos;
	position.z = zPos;
}

void Transform::setScale(double xScale, double yScale, double zScale)
{
	scale.x = xScale;
	scale.y = yScale;
	scale.z = zScale;
}

void Transform::setRotation(double xRot, double yRot, double zRot)
{
	rotation.x = xRot;
	rotation.y = yRot;
	rotation.z = zRot;
}

void Transform::setPosition(Vector3 pos)
{
	position = pos;
}

void Transform::setScale(Vector3 _scale)
{
	scale = _scale;
}

void Transform::setRotation(Vector3 rot)
{
	rotation = rot;
}

Vector3 Transform::getPosition()
{
	return position;
}

Vector3 Transform::getRotation()
{
	return rotation;
}

Vector3 Transform::getScale()
{
	return scale;
}

void Transform::translate(Vector3 pos)
{
	position += pos;

}

void Transform::rotate(Vector3 rot)
{
	rotation += rot;

}

