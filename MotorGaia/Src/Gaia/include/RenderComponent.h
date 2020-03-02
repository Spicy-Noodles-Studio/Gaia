#ifndef RENDER_COMPONENT_H
#define RENDER_COMPONERT_H

#pragma once
#include "GaiaComponent.h"
#include "RenderSystem.h"
#include "utilities.h"

//enum Axis { X, Y, Z };

class RenderComponent : public GaiaComponent
{
private:
	Ogre::SceneNode* node;
	bool visible = true;

	std::map<std::string, Ogre::Entity*> entities;

public:
	RenderComponent(GameObject* gameObject);
	~RenderComponent();

	Ogre::SceneNode* getNode();

	void createEntity(std::string id, std::string mesh);
	void setMaterial(std::string id, std::string material);

	void setVisible(bool visible);
	bool isVisible();

	void addChildNode(std::string tag);
};

#endif