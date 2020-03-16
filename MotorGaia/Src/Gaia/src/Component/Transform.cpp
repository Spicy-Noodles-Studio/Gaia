#include "Transform.h"
#include "GameObject.h"
#include "ComponentData.h"
#include <OgreQuaternion.h>
#include <OgreRoot.h>
#include <sstream>
#include <cmath>
#include "DebugUtils.h"

Transform::Transform(GameObject* gameObject) : GaiaComponent(gameObject)
{
	position.x = 0.0;
	position.y = 0.0;
	position.z = 0.0;

	scale.x = 1.0;
	scale.y = 1.0;
	scale.z = 1.0;

	rotation.x = 0.0;
	rotation.y = 0.0;
	rotation.z = 0.0;

	quaternion = Quaternion::AnglesToQuaternion(0.0, 0.0, 0.0);

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
		Ogre::Matrix3 mx;
		mx.FromEulerAnglesZYX(Ogre::Radian(Ogre::Degree(rotation.z)), Ogre::Radian(Ogre::Degree(rotation.y)), Ogre::Radian(Ogre::Degree(rotation.x)));
		gameObject->node->setOrientation(Ogre::Quaternion(mx));
	}

	quaternion = Quaternion::AnglesToQuaternion(z, y, x);
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
		auto aux = parent->node->convertWorldToLocalPosition(Ogre::Vector3(pos.x, pos.y, pos.z));
		worldPos = { aux.x, aux.y,aux.z };
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
	Vector3 localRotation = rot;
	GameObject* parent = gameObject->getParent();
	if (parent != nullptr) {
		Ogre::Matrix3 mx;
		mx.FromEulerAnglesZYX(Ogre::Radian(Ogre::Degree(rot.z)), Ogre::Radian(Ogre::Degree(rot.y)), Ogre::Radian(Ogre::Degree(rot.x)));
		Ogre::Quaternion aux = parent->node->convertWorldToLocalOrientation(Ogre::Quaternion(mx));

		aux.ToRotationMatrix(mx);
		Ogre::Radian x, y, z; mx.ToEulerAnglesZYX(z, y, x);

		localRotation = { x.valueDegrees(),  y.valueDegrees(), z.valueDegrees() };
		localRotation += {360.0, 360.0, 360.0};
		localRotation = { std::fmod(localRotation.x, 360), std::fmod(localRotation.y, 360) , std::fmod(localRotation.z, 360) };
	}
	setRotation(localRotation);
}

Vector3 Transform::getWorldPosition() const
{
	Vector3 worldPos = position;
	GameObject* parent = gameObject->getParent();
	if (parent != nullptr) {
		auto aux = parent->node->convertLocalToWorldPosition(gameObject->node->getPosition());
		worldPos = { aux.x, aux.y,aux.z };
	}
	return worldPos;
}

Vector3 Transform::getWorldScale() const
{
	Vector3 worldScale = scale;
	GameObject* parent = gameObject->getParent();
	if (parent != nullptr) {
		worldScale *= parent->transform->getWorldScale();
	}
	return worldScale;
}

Vector3 Transform::getWorldRotation() const
{
	Vector3 worldRotation = rotation;
	GameObject* parent = gameObject->getParent();
	if (parent != nullptr) {
		Ogre::Quaternion aux =parent->node->convertLocalToWorldOrientation(gameObject->node->getOrientation()); aux.normalise();
		Ogre::Matrix3 mx; aux.ToRotationMatrix(mx);
		Ogre::Radian x, y, z; mx.ToEulerAnglesZYX(z, y, x);

		worldRotation = { x.valueDegrees(),  y.valueDegrees(), z.valueDegrees() };
		worldRotation += {360.0, 360.0, 360.0};
		worldRotation = { std::fmod(worldRotation.x, 360), std::fmod(worldRotation.y, 360) , std::fmod(worldRotation.z, 360) };
	}
	return worldRotation;
}

Vector3 Transform::getPosition() const
{
	return position;
}

Vector3 Transform::getScale() const
{
	return scale;
}

Vector3 Transform::getRotation() const
{
	return rotation;
}

Quaternion Transform::getQuaternion() const
{
	return quaternion;
}

Vector3 Transform::getForwardVector() const
{
	return GetForwardVector(quaternion);
}

Vector3 Transform::getUpVector() const
{
	return GetUpVector(quaternion);
}

Vector3 Transform::getLeftVector() const
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
		double x, y, z; 
		if (!(ss >> x >> y >> z)) {
			LOG_ERROR("TRANFORM", "invalid value \"%s\"", prop.second.c_str());
			continue;
		}

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
			LOG("TRANSFORM: property %s does not exist\n", prop.first.c_str());
		}
	}
}
