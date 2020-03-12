#include "Transform.h"
#include "GameObject.h"
#include "ComponentData.h"
#include <OgreQuaternion.h>
#include <sstream>
#include "RigidBody.h"

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

	gameObject->transform = this;
}

Transform::~Transform()
{

}

void Transform::setPosition(double x, double y, double z)
{
	position.x = x;
	position.y = y;
	position.z = z;

	if (gameObject->node != nullptr)
		gameObject->node->setPosition(x, y, z);
}

void Transform::setScale(double x, double y, double z)
{
	scale.x = x;
	scale.y = y;
	scale.z = z;

	if (gameObject->node != nullptr)
		gameObject->node->setScale(x, y, z);
}

void Transform::setRotation(double x, double y, double z)
{
	rotation.x = x;
	rotation.y = y;
	rotation.z = z;

	if (gameObject->node != nullptr)
	{
		gameObject->node->setOrientation(Ogre::Quaternion());
		gameObject->node->pitch(Ogre::Radian(Ogre::Degree(x)), Ogre::Node::TS_WORLD);
		gameObject->node->yaw(Ogre::Radian(Ogre::Degree(y)), Ogre::Node::TS_WORLD);
		gameObject->node->roll(Ogre::Radian(Ogre::Degree(z)), Ogre::Node::TS_WORLD);
	}

	quaternion = ToQuaternion(z, y, x);
}

void Transform::setPosition(const Vector3& pos)
{
	setPosition(pos.x, pos.y, pos.z);
}

void Transform::setScale(const Vector3& scale)
{
	setScale(scale.x, scale.y, scale.z);
}

void Transform::setRotation(const Vector3& rot)
{
	setRotation(rot.x, rot.y, rot.z);
}

void Transform::setOrientation(const Ogre::Quaternion& rot)
{
	gameObject->node->setOrientation(rot);
}

void Transform::setWorldPosition(const Vector3& pos)
{
	Vector3 worldPos = pos;
	GameObject* parent = gameObject->getParent();
	if (parent != nullptr) {
		Vector3 parentPos = parent->transform->getWorldPosition();
		worldPos = rotateAroundPivot(worldPos, parentPos, { -parent->transform->getWorldRotation().x, 0,0 });
		worldPos = rotateAroundPivot(worldPos, parentPos, { 0, -parent->transform->getWorldRotation().y,0 });
		worldPos = rotateAroundPivot(worldPos, parentPos, { 0,0, -parent->transform->getWorldRotation().z, });
		worldPos -= parentPos;
		worldPos /= parent->transform->getWorldScale();
	}
	setPosition(worldPos);
}

void Transform::setWorldScale(const Vector3& scale)
{
	Vector3 worldScale = scale;
	GameObject* parent = gameObject->getParent();
	if (parent != nullptr)
		worldScale /= parent->transform->getWorldScale();
	setScale(worldScale);
}

void Transform::setWorldRotation(const Vector3& rot)
{
	Vector3 worldRotation = rot;
	GameObject* parent = gameObject->getParent();
	if (parent != nullptr) {
		Vector3 parentRot = parent->transform->getWorldRotation();
		worldRotation.x = -std::fmod((parentRot.x - worldRotation.x), 360);
		worldRotation.y = std::fmod((parentRot.y - worldRotation.y), 360);
		worldRotation.z = -std::fmod((parentRot.z - worldRotation.z), 360);
	}
	setRotation(worldRotation);
}

const Vector3& Transform::getWorldPosition() const
{
	Vector3 worldPos = position;
	GameObject* parent = gameObject->getParent();
	if (parent != nullptr) {
		Vector3 parentPos = parent->transform->getWorldPosition();
		worldPos *= parent->transform->getWorldScale();
		worldPos += parentPos;
		worldPos = rotateAroundPivot(worldPos, parentPos, { parent->transform->getWorldRotation().x, 0,0 });
		worldPos = rotateAroundPivot(worldPos, parentPos, { 0, parent->transform->getWorldRotation().y,0 });
		worldPos = rotateAroundPivot(worldPos, parentPos, { 0,0, parent->transform->getWorldRotation().z, });
	}
	return worldPos;
}

const Vector3& Transform::getWorldScale() const
{
	Vector3 worldScale = scale;
	GameObject* parent = gameObject->getParent();
	if (parent != nullptr)
		worldScale = worldScale * parent->transform->getWorldScale();
	return worldScale;
}

const Vector3& Transform::getWorldRotation() const
{
	Vector3 worldRotation = rotation;
	GameObject* parent = gameObject->getParent();
	if (parent != nullptr) {
		Vector3 parentRot = parent->transform->getWorldRotation();
		worldRotation.x = -std::fmod((worldRotation.x + parentRot.x), 360);
		worldRotation.y = std::fmod((worldRotation.y + parentRot.y), 360);
		worldRotation.z = -std::fmod((worldRotation.z + parentRot.z), 360);
	}
	return worldRotation;
}

const Vector3& Transform::getPosition() const
{
	return position;
}

const Vector3& Transform::getScale() const
{
	return scale;
}

const Vector3& Transform::getRotation() const
{
	return rotation;
}

const Vector3& Transform::getForwardVector() const
{
	return GetForwardVector(quaternion);
}

const Vector3& Transform::getUpVector() const
{
	return GetUpVector(quaternion);
}

const Vector3& Transform::getLeftVector() const
{
	return GetLeftVector(quaternion);
}

void Transform::translate(const Vector3& pos)
{
	position += pos;
	setPosition(position);
}

void Transform::rotate(const Vector3& rot)
{
	rotation += rot;
	setRotation(rotation);
}

void Transform::handleData(ComponentData* data)
{
	for (auto prop : data->getProperties()) {
		std::stringstream ss(prop.second);
		double x, y, z; ss >> x >> y >> z;

		if (prop.first == "position") {
			setPosition(x, y, z);
		}
		else if (prop.first == "scale") {
			setScale(x, y, z);
		}
		else if (prop.first == "rotation") {
			setRotation(x, y, z);
		}
		else {
			printf("TRANSFORM: property %s does not exist\n", prop.first.c_str());
		}
	}
}
