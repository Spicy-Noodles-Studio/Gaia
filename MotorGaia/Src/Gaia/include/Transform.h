#pragma once
#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "GaiaComponent.h"
#include "utilities.h"

class Transform : public GaiaComponent
{
private:
	Vector3 position;
	Vector3 rotation;
	Vector3 scale;

public:
	Transform(GameObject* gameObject);
	~Transform();

	void setPosition(double xPos, double yPos, double zPos);
	void setScale(double xScale, double yScale, double zScale);
	void setRotation(double xRot, double yRot, double zRot);

	void setPosition(Vector3 pos);
	void setScale(Vector3 _scale);
	void setRotation(Vector3 rot);

	Vector3 getPosition();
	Vector3 getRotation();
	Vector3 getScale();

	void translate(Vector3 pos);
	void rotate(Vector3 rot);

};

#endif

