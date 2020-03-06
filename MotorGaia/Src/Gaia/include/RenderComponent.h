#pragma once

#include "GaiaComponent.h"
#include "RenderSystem.h"

enum Axis { X, Y, Z };

class RenderComponent : public GaiaComponent
{
private:
	Ogre::SceneNode* node;
	std::map<std::string, Ogre::Entity*> entities;
	std::map<std::string, Ogre::SceneNode*> nodes;
	std::map<std::string, bool> nodesVisibility;

public:
	RenderComponent(GameObject* gameObject);
	~RenderComponent();

	void createEntity(Ogre::Entity* entity, std::string mesh, std::string id);
	void setMaterial(Ogre::Entity* entity, std::string material);
	void createChildNode(Ogre::Entity* entity, std::string id);

	void setPosition(Ogre::SceneNode* node, Ogre::Vector3 position);
	void setScale(Ogre::SceneNode* node, Ogre::Vector3 scale);
	void setRotation(Ogre::SceneNode* node, Axis axis, float degrees);

	void setVisible(Ogre::SceneNode* node, bool visible);
	bool isVisible(std::string id);
};