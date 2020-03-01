#include "TransformComponent.h"

TransformComponent::TransformComponent(GameObject* gameObject):GaiaComponent(gameObject)
{
	position.x = 0;
	position.y = 0;
	position.z = 0;

	positionRender.x = 0;
	positionRender.y = 0;
	positionRender.z = 0;
	scale.x = 1;
	scale.y = 1;
	scale.z = 1;

	scaleRender.x = 1;
	scaleRender.y = 1;
	scaleRender.z = 1;

	rotation.x = 0;
	rotation.y = 0;
	rotation.z = 0;

	rotationRender.x = 0;
	rotationRender.y = 0;
	rotationRender.z = 0;

}

TransformComponent::~TransformComponent()
{
}



void TransformComponent::setPosition(double xPos, double yPos, double zPos)
{
	position.x = xPos;
	position.y = yPos;
	position.z = zPos;

	positionRender.x = xPos;
	positionRender.y = yPos;
	positionRender.z = zPos;
}

void TransformComponent::setScale(double xScale, double yScale, double zScale)
{
	scale.x = xScale;
	scale.y = yScale;
	scale.z = zScale;

	scaleRender.x = xScale;
	scaleRender.y = yScale;
	scaleRender.z = zScale;
}

void TransformComponent::setRotation(double xRot, double yRot, double zRot)
{
	rotation.x = xRot;
	rotation.y = yRot;
	rotation.z = zRot;

	rotationRender.x = xRot;
	rotationRender.y = yRot;
	rotationRender.z = zRot;
}

void TransformComponent::setPosition(Vector3D pos)
{
	setPosition(pos.x, pos.y, pos.z);
}

void TransformComponent::setScale(Vector3D scale)
{
	setScale(scale.x, scale.y, scale.z);
}

void TransformComponent::setRotation(Vector3D rot)
{
	setRotation(rot.x, rot.y, rot.z);
}

Vector3D TransformComponent::getTransformPos()
{
	return position;
}

Vector3D TransformComponent::getTransformRot()
{
	return rotation;
}

Vector3D TransformComponent::getTransformScale()
{
	return scale;
}

Ogre::Vector3 TransformComponent::getRenderPos()
{
	return positionRender;
}

Ogre::Vector3 TransformComponent::getRenderRot()
{
	return rotationRender;
}

Ogre::Vector3 TransformComponent::getRenderScale()
{
	return scaleRender;
}
