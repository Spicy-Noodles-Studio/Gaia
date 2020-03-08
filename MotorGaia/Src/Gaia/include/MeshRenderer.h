#pragma once
#ifndef MESH_RENDERER_H
#define MESH_RENDERER_H

#include "GaiaComponent.h"
#include "RenderSystem.h"

class MeshRenderer : public GaiaComponent
{
private:
	std::map<std::string, Ogre::Entity*> entities;
	bool visible;

public:
	MeshRenderer(GameObject* gameObject);
	~MeshRenderer();

	Ogre::Entity* getMesh(std::string mesh);
	
	void setMesh(const std::string& id, const std::string& mesh);
	void setMaterial(const std::string& id, const std::string& material);

	void attachEntityToNode(const std::string& mesh);
	void attachEntityToBone(const std::string& owner, const std::string& bone, const std::string& mesh);

	void setVisible(bool visible);
	bool isVisible();

	virtual void handleData(ComponentData* data);
};

#endif