#pragma once
#ifndef CAMERA_H
#define CAMERA_H

#include "GaiaComponent.h"
#include "RenderSystem.h"
#include "Vector3.h"

class Camera : public GaiaComponent
{
private:
	Ogre::Camera* camera;
	bool isMainCamera;

public:
	Camera(GameObject* gameObject);
	~Camera();

	enum SpaceReference { LocalSpace, WorldSpace, ParentSpace };

	void lookAt(const Vector3& pos, SpaceReference space);
	void setDirection(const Vector3& dir);

	Ogre::Camera* getCamera();

	void setClipDistances(double near, double far);

	virtual void handleData(ComponentData* data);
};

#endif