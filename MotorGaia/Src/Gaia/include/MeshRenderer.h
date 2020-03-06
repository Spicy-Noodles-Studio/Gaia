#pragma once
#ifndef MESH_RENDERER_H
#define MESH_RENDERER_H

#include "GaiaComponent.h"
#include "RenderSystem.h"

class MeshRenderer : public GaiaComponent
{
private:
	std::map<std::string, Ogre::Entity*> entities;
	bool visible = true;

public:
	MeshRenderer(GameObject* gameObject);
	~MeshRenderer();

	Ogre::Entity* getEntity(std::string entity);

	void createEntity(std::string id, std::string mesh);
	void setMaterial(std::string id, std::string material);

	void attachEntityToNode(std::string entity);
	void attachEntityToBone(std::string owner, std::string bone, std::string entity);

	void setVisible(bool visible);
	bool isVisible();
};

#endif