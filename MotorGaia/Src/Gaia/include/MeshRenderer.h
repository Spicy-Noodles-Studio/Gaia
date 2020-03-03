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

	void createEntity(std::string id, std::string mesh);
	void setMaterial(std::string id, std::string material);

	void setVisible(bool visible);
	bool isVisible();
};

#endif