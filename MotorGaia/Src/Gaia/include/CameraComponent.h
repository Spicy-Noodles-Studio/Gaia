#pragma once
#include "GaiaComponent.h"
#include "RenderSystem.h"
#include "utilities.h"

class CameraComponent : public GaiaComponent
{
private:
	Ogre::SceneNode* node;
	Ogre::Camera* cam;

public:
	CameraComponent(GameObject* gameObject, std::string name);
	~CameraComponent();

	void setPosition(Ogre::SceneNode* node, Ogre::Vector3 position);
	
	void setRotation(Ogre::SceneNode* node, Axis axis, float degrees);

	void lookAt(Ogre::Vector3 pos, Ogre::Node::TransformSpace space);
	void setDirecction(Ogre::Vector3 dir);

	//Solo falta añadirla al viewport
	Ogre::Camera* getCamera();

	void setClipDistances(double near, double far);
};