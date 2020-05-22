#pragma once
#ifndef MESH_RENDERER_H
#define MESH_RENDERER_H

#include "GaiaComponent.h"
#include "Vector3.h"

#include <map>

namespace Ogre
{
	class Entity;
}

class GAIA_API MeshRenderer : public GaiaComponent
{
	friend class Animator;
	friend class Strider;
	friend class Trail;
private:
	std::map<std::string, Ogre::Entity*> entities;
	bool visible;
	std::string meshId, meshName; // The id and name of the current mesh

public:
	MeshRenderer(GameObject* gameObject);
	virtual ~MeshRenderer();

	const std::string& getMeshId() const;
	const std::string& getMeshName() const;

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
	bool isVisible() const;

	void printAllBones() const;

	void setDiffuse(int subentity, const Vector3& diffuse, float alpha);
	void setDiffuse(const std::string& entity, int subentity, const Vector3& diffuse, float alpha);

	void setFpParam(int subentity, const std::string& param, float value);

	Vector3 getDiffuse(int subentity) const;
	Vector3 getDiffuse(std::string entity, int subentity) const;

	void setEmissive(int subentity, const Vector3& emissive);
	Vector3 getEmissive(int subentity) const;

	int getSubentitiesSize() const;

	void setTexture(int subentity, const std::string& textureName);
	const std::string& getTexture(int subentity) const;

protected:
	virtual void handleData(ComponentData* data);

private:
	Ogre::Entity* getMesh(const std::string& mesh) const;
};

#endif