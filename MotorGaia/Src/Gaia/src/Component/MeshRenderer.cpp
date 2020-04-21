#include "MeshRenderer.h"

#include <OgreRoot.h>
#include <OgreSceneManager.h>
#include <OgreSkeletonInstance.h>
#include <OgreSkeleton.h>
#include <OgreBone.h>
#include <OgreMesh.h>
#include <OgreEntity.h>
#include <OgreSubEntity.h>
#include <OgreMaterial.h>
#include <OgreTechnique.h>
#include <OgrePass.h>
#include <OgreMaterialManager.h>

#include <sstream>

#include "GameObject.h"
#include "Scene.h"
#include "ComponentData.h"
#include "ComponentRegister.h"
#include "Animator.h"

REGISTER_FACTORY(MeshRenderer);

MeshRenderer::MeshRenderer(GameObject* gameObject) : GaiaComponent(gameObject), visible(true)
{
	 
}

MeshRenderer::~MeshRenderer()
{
	for (auto entity : entities) {
		gameObject->node->detachObject(entity.second);
		gameObject->getScene()->getSceneManager()->destroyEntity(entity.second);
	}

	entities.clear();
}

Ogre::Entity* MeshRenderer::getMesh(std::string mesh)
{
	return entities[mesh];
}

std::string MeshRenderer::getMeshId() const
{
	return meshId;
}

std::string MeshRenderer::getMeshName() const
{
	return meshName;
}

void MeshRenderer::setMesh(const std::string& id, const std::string& mesh)
{
	if (entities.find(id) == entities.end())
		entities[id] = gameObject->getScene()->createEntity(mesh);
}

void MeshRenderer::setMaterial(const std::string& id, const std::string& material)
{
	if (entities.find(id) != entities.end())
		entities[id]->setMaterialName(material);
}

void MeshRenderer::setMaterial(const std::string& id, int subentity, const std::string& material)
{
	if (entities.find(id) != entities.end())
		entities[id]->getSubEntity(subentity)->setMaterialName(material);
}

void MeshRenderer::changeMesh(const std::string& id, const std::string& mesh)
{
	// Dettach previously attached meshes
	gameObject->node->detachAllObjects();

	setMesh(id, mesh);
	attachEntityToNode(id);
	meshId = id;
	meshName = mesh;

	// Change animations if the mesh has animations
	auto anim = gameObject->getComponent<Animator>();
	if (anim != nullptr)	anim->setMesh(id);
}

void MeshRenderer::attachEntityToNode(const std::string& mesh)
{
	if (entities.find(mesh) != entities.end())
		gameObject->node->attachObject(entities[mesh]);
	else
		LOG("MESH RENDERER: Trying to attach non existing mesh %s", mesh.c_str());
}

void MeshRenderer::attachEntityToBone(const std::string& owner, const std::string& bone, const std::string& mesh)
{
	auto ownerIt = entities.find(owner), meshIt = entities.find(mesh);
	if (ownerIt != entities.end() && meshIt != entities.end()) {
		Ogre::Entity* ownerEnt = (*ownerIt).second, * meshEnt = (*meshIt).second;
		if (ownerEnt->hasSkeleton() && ownerEnt->getSkeleton()->hasBone(bone))
			ownerEnt->attachObjectToBone(bone, meshEnt);
		else
			LOG("MESH RENDERER: The mesh %s does not have a skeleton or bone %s does not exist", owner.c_str(), bone.c_str());
	}
	else
		LOG("MESH RENDERER: One of the meshes specified does not exist in object %s", gameObject->getName().c_str());
}

void MeshRenderer::detachEntityFromBone(const std::string& owner, const std::string& mesh)
{
	auto ownerIt = entities.find(owner), meshIt = entities.find(mesh);
	if (ownerIt != entities.end() && meshIt != entities.end()) {
		Ogre::Entity* ownerEnt = (*ownerIt).second, * meshEnt = (*meshIt).second;
		if (ownerEnt->hasSkeleton())
			ownerEnt->detachObjectFromBone(meshEnt);
		else
			LOG("MESH RENDERER: The mesh %s does not have a skeleton", owner.c_str());
	}
	else
		LOG("MESH RENDERER: The mesh specified does not exist in object %s", gameObject->getName().c_str());
}

void MeshRenderer::moveEntityToBone(const std::string& owner, const std::string& bone, const std::string& mesh)
{
	detachEntityFromBone(owner, mesh);
	attachEntityToBone(owner, bone, mesh);
}

void MeshRenderer::setVisible(bool visible)
{
	gameObject->node->setVisible(visible, false);
	this->visible = visible;
}

bool MeshRenderer::isVisible()
{
	return visible;
}

void MeshRenderer::printAllBones()
{
	printf("%s BONES:\n", meshName.c_str());
	auto skeleton = getMesh(meshId)->getMesh()->getSkeleton();
	auto numBones = skeleton->getNumBones();
	for (int i = 0; i < numBones; i++)
	{
		printf("%s\n", skeleton->getBone(i)->getName().c_str());
	}
	printf("\n");
}

void MeshRenderer::setDiffuse(int subentity, const Vector3& diffuse, float alpha)
{
	Ogre::MaterialPtr mat = getMesh(meshId)->getSubEntity(subentity)->getMaterial();
	Ogre::MaterialPtr newMat = Ogre::MaterialManager::getSingleton().getByName(mat->getName() + gameObject->getName());
	if (newMat == NULL) newMat = mat->clone(mat->getName() + gameObject->getName());
	newMat->getTechnique(0)->getPass(0)->setDiffuse(diffuse.x, diffuse.y, diffuse.z, alpha);
	setMaterial(meshId, subentity, newMat->getName());
}

Vector3 MeshRenderer::getDiffuse(int subentity)
{
	Ogre::ColourValue c = getMesh(meshId)->getSubEntity(subentity)->getMaterial()->getTechnique(0)->getPass(0)->getDiffuse();
	return { c.r, c.g, c.b };
}

int MeshRenderer::getSubentitiesSize()
{
	return getMesh(meshId)->getSubEntities().size();
}

void MeshRenderer::setTexture(int subentity, const std::string& textureName)
{
	getMesh(meshId)->getSubEntity(subentity)->getMaterial()->getTechnique(0)->getPass(0)->getTextureUnitState(0)->setTextureName(textureName);
}

std::string MeshRenderer::getTexture(int subentity)
{
	return getMesh(meshId)->getSubEntity(subentity)->getMaterial()->getTechnique(0)->getPass(0)->getTextureUnitState(0)->getTextureName();
}

void MeshRenderer::handleData(ComponentData* data)
{
	for (auto prop : data->getProperties())
	{
		std::stringstream ss(prop.second);

		if (prop.first == "mesh")
		{
			char c;
			while (ss >> meshId >> meshName) {
				setMesh(meshId, meshName);
				attachEntityToNode(meshId);
				if (ss) ss >> c;
			}
		}
		else if (prop.first == "attachToBone")
		{
			//Al ser un attach to bone no modificamos meshId o meshName->no es una mesh principal
			std::string id, name, bone, owner;
			char c;
			while (ss >> id >> name >> owner >> bone) {
				setMesh(id, name);
				attachEntityToBone(owner, bone, id);
				if (ss) ss >> c;
			}
		}
		else if (prop.first == "material")
		{
			std::string id, name;
			if (ss >> id >> name)
				setMaterial(id, name);
			else
				LOG("MESH RENDERER: wrong value for property %s.\n", prop.first.c_str());
		}
		else if (prop.first == "visible")
		{
			bool visible;
			if (ss >> visible)
				setVisible(visible);
			else
				LOG("MESH RENDERER: wrong value for property %s.\n", prop.first.c_str());
		}
	}
}