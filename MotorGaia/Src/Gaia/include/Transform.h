#pragma once
#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "GaiaComponent.h"
#include "Vector3.h"

class Transform : public GaiaComponent
{
private:
	Vector3 position;
	Vector3 rotation;
	Vector3 scale;

public:
	Transform(GameObject* gameObject);
	~Transform();

	void setPosition(double x, double y, double z);
	void setScale(double x, double y, double z);
	void setRotation(double x, double y, double z);

	void setPosition(const Vector3& pos);
	void setScale(const Vector3& scale);
	void setRotation(const Vector3& rot);

	const Vector3& getPosition() const;
	const Vector3& getScale() const;
	const Vector3& getRotation() const;

	void translate(const Vector3& pos);
	void rotate(const Vector3& rot);

	virtual void handleData(ComponentData* data);
};

#endif