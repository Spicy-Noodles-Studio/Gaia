#pragma once
#ifndef MESH_RENDERER_H
#define MESH_RENDERER_H

#include "GaiaComponent.h"
#include <map>

namespace Ogre {
	class Entity;
}

class GAIA_API MeshRenderer : public GaiaComponent
{
private:
	std::map<std::string, Ogre::Entity*> entities;
	bool visible;
	// The id and name of the current mesh
	std::string meshId, meshName;
public:
	MeshRenderer(GameObject* gameObject);
	~MeshRenderer();

	Ogre::Entity* getMesh(std::string mesh);
	std::string getMeshId() const;
	std::string getMeshName() const;
	
	void setMesh(const std::string& id, const std::string& mesh);
	void setMaterial(const std::string& id, const std::string& material);
	void changeMesh(const std::string& id, const std::string& mesh);

	void attachEntityToNode(const std::string& mesh);
	void attachEntityToBone(const std::string& owner, const std::string& bone, const std::string& mesh);

	void setVisible(bool visible);
	bool isVisible();

	virtual void handleData(ComponentData* data);
};

#endif