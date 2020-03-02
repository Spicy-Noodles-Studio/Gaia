#pragma once
#ifndef  LIGHT_H
#define LIGHT_H
#include "GaiaComponent.h"
#include "RenderSystem.h"
#include "utilities.h"

class Light : public GaiaComponent
{
private:
	Ogre::SceneNode* node;
	Ogre::Light* light;
public:
	Light(GameObject* gameObject);
	~Light();

	void setType(LightType type);
	void setColour(float red, float green, float blue);
	void setSpecularColour(float red, float green, float blue);

	void setPosition(Ogre::SceneNode* node, Ogre::Vector3 position);
	void setRotation(Ogre::SceneNode* node, Axis axis, float degrees);
	void setDirecction(Ogre::Vector3 dir);

	void setShadowsDistance(double far);

	void setVisible(bool invisible);
	bool isVisible();
};
#endif