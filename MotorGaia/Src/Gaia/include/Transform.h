#pragma once
#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "GaiaComponent.h"
#include "Quaternion.h"
#include "Vector3.h"

namespace Ogre {
	class Quaternion;
}

class GAIA_API Transform : public GaiaComponent
{
private:
	Vector3 position;
	Vector3 rotation;
	Vector3 scale;
	Quaternion quaternion;

public:
	Transform(GameObject* gameObject);
	~Transform();

	void setPosition(double x, double y, double z);
	void setScale(double x, double y, double z);
	void setRotation(double x, double y, double z);

	void setPosition(const Vector3& pos);
	void setScale(const Vector3& scale);
	void setRotation(const Vector3& rot);
	void setOrientation(const Ogre::Quaternion& rot);

	void setWorldPosition(const Vector3& pos);
	void setWorldScale(const Vector3& scale);
	void setWorldRotation(const Vector3& rot);

	Vector3 getPosition() const;
	Vector3 getScale() const;
	Vector3 getRotation() const;
	Quaternion getQuaternion() const;

	Vector3 getWorldPosition() const;
	Vector3 getWorldScale() const;
	Vector3 getWorldRotation() const;

	Vector3 getForwardVector() const;
	Vector3 getUpVector() const;
	Vector3 getLeftVector() const;

	void translate(const Vector3& pos);
	void rotate(const Vector3& rot);

	virtual void handleData(ComponentData* data);
};

#endif