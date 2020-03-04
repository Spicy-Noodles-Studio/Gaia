#include "Transform.h"
#include "GameObject.h"
#include "ComponentData.h"
#include <OgreQuaternion.h>
#include <sstream>

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

	gameObject->node->setOrientation(Ogre::Quaternion());

	if (gameObject->node != nullptr)
	{
		gameObject->node->pitch(Ogre::Radian(Ogre::Degree(x)));
		gameObject->node->yaw(Ogre::Radian(Ogre::Degree(y)));
		gameObject->node->roll(Ogre::Radian(Ogre::Degree(z)));
	}
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
