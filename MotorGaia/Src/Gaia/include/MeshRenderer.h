#pragma once
#ifndef MESH_RENDERER_H
#define MESH_RENDERER_H

#include "GaiaComponent.h"
#include <map>
#include "Vector3.h"

namespace Ogre 
{
	class Entity;
}

class GAIA_API MeshRenderer : public GaiaComponent
{
private:
	std::map<std::string, Ogre::Entity*> entities;
	bool visible;
	std::string meshId, meshName;// The id and name of the current mesh

public:
	MeshRenderer(GameObject* gameObject);
	~MeshRenderer();

	Ogre::Entity* getMesh(std::string mesh);
	std::string getMeshId() const;
	std::string getMeshName() const;

	void setMesh(const std::string& id, const std::string& mesh);

	void setMaterial(const std::string& id, const std::string& material);
	void setMaterial(const std::string& id, int subentity, const std::string& material);
	void changeMesh(const std::string& id, const std::string& mesh);

	void attachEntityToNode(const std::string& mesh);
	void attachEntityToBone(const std::string& owner, const std::string& bone, const std::string& mesh);
	void detachEntityFromBone(const std::string& owner, const std::string& mesh);
	// Move an entity mesh from one bone to the bone 'bone' of owners' skeleton
	void moveEntityToBone(const std::string& owner, const std::string& bone, const std::string& mesh);

	void setVisible(bool visible);
	bool isVisible();

	void printAllBones();

	void setDiffuse(int subentity, const Vector3& diffuse, float alpha);
	Vector3 getDiffuse(int subentity);

	int getSubentitiesSize();

	void setTexture(int subentity, const std::string& textureName);
	std::string getTexture(int subentity);

	virtual void handleData(ComponentData* data);
};

#endif