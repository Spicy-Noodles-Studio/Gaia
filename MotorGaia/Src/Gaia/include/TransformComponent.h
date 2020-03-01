#pragma once


#include "GaiaComponent.h"
#include <OgreRoot.h>

struct Vector3D {
	double x;
	double y;
	double z;
};
class TransformComponent : public GaiaComponent
{

private:



	Vector3D position;
	Vector3D rotation;
	Vector3D scale;
	Ogre::Vector3 positionRender;
	Ogre::Vector3 rotationRender;
	Ogre::Vector3 scaleRender;

public:
	TransformComponent(GameObject* gameObject);
	~TransformComponent();



	void setPosition(double xPos, double yPos, double zPos);
	void setScale(double xScale, double yScale, double zScale);
	void setRotation(double xRot, double yRot, double zRot);

	void setPosition(Vector3D pos);
	void setScale(Vector3D scale);
	void setRotation(Vector3D rot);

	Vector3D getTransformPos();
	Vector3D getTransformRot();
	Vector3D getTransformScale();

	Ogre::Vector3 getRenderPos();
	Ogre::Vector3 getRenderRot();
	Ogre::Vector3 getRenderScale();

};



