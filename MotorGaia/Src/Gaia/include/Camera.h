#pragma once
#ifndef  CAMERA_H
#define CAMERA_H
#include "GaiaComponent.h"
#include "RenderSystem.h"
#include "utilities.h"

class Camera : public GaiaComponent
{
private:
	Ogre::SceneNode* node;
	Ogre::Camera* cam;

public:
	Camera(GameObject* gameObject);
	~Camera();

	void setPosition(Ogre::SceneNode* node, Ogre::Vector3 position);
	
	void setRotation(Ogre::SceneNode* node, Axis axis, float degrees);

	void lookAt(Ogre::Vector3 pos, SpaceReference space);
	void setDirecction(Ogre::Vector3 dir);

	Ogre::Camera* getCamera();

	void setClipDistances(double near, double far);
};
#endif