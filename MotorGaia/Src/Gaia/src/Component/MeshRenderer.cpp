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

MeshRenderer::MeshRenderer(GameObject* gameObject) : GaiaComponent(gameObject), entities(), meshId(""), meshName(""), visible(true)
{

}

MeshRenderer::~MeshRenderer()
{
	checkNullAndBreak(gameObject);
	checkNullAndBreak(gameObject->node);
	Scene* scene = gameObject->getScene();
	checkNullAndBreak(scene);
	Ogre::SceneManager* sceneManager = scene->getSceneManager();
	checkNullAndBreak(sceneManager);

	for (auto entity : entities)
	{
		gameObject->node->detachObject(entity.second);
		sceneManager->destroyEntity(entity.second);
	}

	entities.clear();
}

const std::string& MeshRenderer::getMeshId() const
{
	return meshId;
}

const std::string& MeshRenderer::getMeshName() const
{
	return meshName;
}

void MeshRenderer::setMesh(const std::string& id, const std::string& mesh)
{
	if (entities.find(id) != entities.end())
		LOG_ERROR("MESH RENDERER", "Given ID already in use. Overwritting mesh if not the same");

	checkNullAndBreak(gameObject);
	Scene* scene = gameObject->getScene();
	checkNullAndBreak(scene);
	Ogre::Entity* entity = scene->createEntity(mesh);
	checkNullAndBreak(entity);
	entities[id] = entity;
}

void MeshRenderer::setMaterial(const std::string& id, const std::string& material)
{
	if (entities.find(id) == entities.end()) {
		LOG_ERROR("MESH RENDERER", "No mesh exits with given ID");
		return;
	}
	checkNullAndBreak(entities[id]);

	entities[id]->setMaterialName(material);
}

void MeshRenderer::setMaterial(const std::string& id, int subentity, const std::string& material)
{
	if (entities.find(id) == entities.end()) {
		LOG_ERROR("MESH RENDERER", "No mesh exits with given ID");
		return;
	}
	checkNullAndBreak(entities[id]);
	Ogre::SubEntity* subEntity = entities[id]->getSubEntity(subentity);
	checkNullAndBreak(subEntity);
	subEntity->setMaterialName(material);
}

void MeshRenderer::changeMesh(const std::string& id, const std::string& mesh)
{
	checkNullAndBreak(gameObject);
	checkNullAndBreak(gameObject->node);
	// Dettach previously attached meshes
	gameObject->node->detachAllObjects();

	setMesh(id, mesh);
	attachEntityToNode(id);

	meshId = id;
	meshName = mesh;

	// Change animations if the mesh has animations
	Animator* animator = gameObject->getComponent<Animator>();
	checkNullAndBreak(animator);
	if (animator != nullptr)	animator->setMesh(id);
}

void MeshRenderer::attachEntityToNode(const std::string& mesh)
{
	if (entities.find(mesh) == entities.end()) {
		LOG_ERROR("MESH RENDERER", "Trying to attach non existing mesh %s", mesh.c_str());
		return;
	}
	checkNullAndBreak(gameObject);
	checkNullAndBreak(gameObject->node);
	checkNullAndBreak(entities[mesh]);

	gameObject->node->attachObject(entities[mesh]);
}

void MeshRenderer::attachEntityToBone(const std::string& owner, const std::string& bone, const std::string& mesh)
{
	auto ownerIt = entities.find(owner), meshIt = entities.find(mesh);
	if (ownerIt != entities.end() && meshIt != entities.end())
	{
		Ogre::Entity* ownerEntity = (*ownerIt).second;
		Ogre::Entity *meshEntity = (*meshIt).second;
		checkNullAndBreak(ownerEntity);
		checkNullAndBreak(meshEntity);

		Ogre::SkeletonInstance* skeleton = ownerEntity->getSkeleton();
		checkNullAndBreak(skeleton);

		if (ownerEntity->hasSkeleton() && skeleton->hasBone(bone))
			ownerEntity->attachObjectToBone(bone, meshEntity);
		else
			LOG_ERROR("MESH RENDERER", "The mesh %s does not have a skeleton or bone %s does not exist", owner.c_str(), bone.c_str());
	}
	else {
		checkNullAndBreak(gameObject);
		LOG_ERROR("MESH RENDERER", "One of the meshes specified does not exist in object %s", gameObject->getName().c_str());
	}
}

void MeshRenderer::detachEntityFromBone(const std::string& owner, const std::string& mesh)
{
	auto ownerIt = entities.find(owner), meshIt = entities.find(mesh);
	if (ownerIt != entities.end() && meshIt != entities.end())
	{
		Ogre::Entity* ownerEntity = (*ownerIt).second;
		Ogre::Entity* meshEntity = (*meshIt).second;
		checkNullAndBreak(ownerEntity);
		checkNullAndBreak(meshEntity);

		if (ownerEntity->hasSkeleton())
			ownerEntity->detachObjectFromBone(meshEntity);
		else
			LOG_ERROR("MESH RENDERER", "The mesh %s does not have a skeleton", owner.c_str());
	}
	else {
		checkNullAndBreak(gameObject);
		LOG_ERROR("MESH RENDERER", "The mesh specified does not exist in object %s", gameObject->getName().c_str());
	}
}

void MeshRenderer::moveEntityToBone(const std::string& owner, const std::string& bone, const std::string& mesh)
{
	detachEntityFromBone(owner, mesh);
	attachEntityToBone(owner, bone, mesh);
}

void MeshRenderer::setVisible(bool visible)
{
	checkNullAndBreak(gameObject);
	checkNullAndBreak(gameObject->node);

	gameObject->node->setVisible(visible, false);
	this->visible = visible;
}

bool MeshRenderer::isVisible() const
{
	return visible;
}

void MeshRenderer::printAllBones() const
{
	Ogre::Entity* entity = getMesh(meshId);
	checkNullAndBreak(entity);
	Ogre::MeshPtr meshPtr = entity->getMesh();
	checkNullAndBreak(meshPtr.get());
	Ogre::SkeletonPtr skeleton = meshPtr->getSkeleton();
	checkNullAndBreak(skeleton.get());

	auto numBones = skeleton->getNumBones();
	LOG("%s BONES:", meshName.c_str());
	for (int i = 0; i < numBones; i++) {
		Ogre::Bone* bone = skeleton->getBone(i);
		checkNullAndBreak(bone);
		LOG("%s", bone->getName().c_str());
	}
	LOG("");
}

void MeshRenderer::setDiffuse(int subentity, const Vector3& diffuse, float alpha)
{
	setDiffuse(meshId, subentity, diffuse, alpha);
}

void MeshRenderer::setDiffuse(const std::string& entity, int subentity, const Vector3& diffuse, float alpha)
{
	checkNullAndBreak(gameObject);

	Ogre::Entity* mEntity = getMesh(entity);
	checkNullAndBreak(mEntity);
	Ogre::SubEntity* subEntity = mEntity->getSubEntity(subentity);
	checkNullAndBreak(subEntity);

	Ogre::MaterialPtr material = subEntity->getMaterial();
	checkNullAndBreak(material.get());

	Ogre::MaterialPtr newMaterial = Ogre::MaterialManager::getSingleton().getByName(material->getName() + gameObject->getName());
	if (newMaterial == NULL) 
		newMaterial = material->clone(material->getName() + gameObject->getName());
	checkNullAndBreak(newMaterial.get());

	Ogre::Technique* technique = newMaterial->getTechnique(0);
	checkNullAndBreak(technique);
	Ogre::Pass* pass = technique->getPass(0);
	checkNullAndBreak(pass);

	pass->setDiffuse(diffuse.x, diffuse.y, diffuse.z, alpha);
	setMaterial(entity, subentity, newMaterial->getName());
}

void MeshRenderer::setFpParam(int subentity, const std::string& param, float value)
{
	checkNullAndBreak(gameObject);

	Ogre::Entity* mEntity = getMesh(meshId);
	checkNullAndBreak(mEntity);
	Ogre::SubEntity* subEntity = mEntity->getSubEntity(subentity);
	checkNullAndBreak(subEntity);

	Ogre::MaterialPtr material = subEntity->getMaterial();
	checkNullAndBreak(material.get());

	Ogre::MaterialPtr newMaterial = Ogre::MaterialManager::getSingleton().getByName(material->getName() + gameObject->getName());
	if (newMaterial == NULL)
		newMaterial = material->clone(material->getName() + gameObject->getName());
	checkNullAndBreak(newMaterial.get());

	Ogre::Technique* technique = newMaterial->getTechnique(0);
	checkNullAndBreak(technique);
	Ogre::Pass* pass = technique->getPass(0);
	checkNullAndBreak(pass);

	Ogre::GpuProgramParametersSharedPtr params = pass->getFragmentProgramParameters();
	checkNullAndBreak(params.get());

	params->setNamedConstant(param, value);
	setMaterial(meshId, subentity, newMaterial->getName());
}

Vector3 MeshRenderer::getDiffuse(int subentity) const
{
	return getDiffuse(meshId, subentity);
}

Vector3 MeshRenderer::getDiffuse(std::string entity, int subentity) const
{
	Ogre::Entity* mEntity = getMesh(entity);
	checkNullAndBreak(mEntity, Vector3::ZERO);
	Ogre::SubEntity* subEntity = mEntity->getSubEntity(subentity);
	checkNullAndBreak(subEntity, Vector3::ZERO);

	Ogre::MaterialPtr material = subEntity->getMaterial();
	checkNullAndBreak(material.get(), Vector3::ZERO);

	Ogre::Technique* technique = material->getTechnique(0);
	checkNullAndBreak(technique, Vector3::ZERO);
	Ogre::Pass* pass = technique->getPass(0);
	checkNullAndBreak(pass, Vector3::ZERO);

	Ogre::ColourValue c = pass->getDiffuse();
	return { c.r, c.g, c.b };
}

void MeshRenderer::setEmissive(int subentity, const Vector3& emissive)
{
	checkNullAndBreak(gameObject);

	Ogre::Entity* entity = getMesh(meshId);
	checkNullAndBreak(entity);
	Ogre::SubEntity* subEntity = entity->getSubEntity(subentity);
	checkNullAndBreak(subEntity);

	Ogre::MaterialPtr material = subEntity->getMaterial();
	checkNullAndBreak(material.get());

	Ogre::MaterialPtr newMaterial = Ogre::MaterialManager::getSingleton().getByName(material->getName() + gameObject->getName());
	if (newMaterial == NULL) newMaterial = material->clone(material->getName() + gameObject->getName());
	checkNullAndBreak(newMaterial.get());

	Ogre::Technique* technique = newMaterial->getTechnique(0);
	checkNullAndBreak(technique);
	Ogre::Pass* pass = technique->getPass(0);
	checkNullAndBreak(pass);

	pass->setEmissive(emissive.x, emissive.y, emissive.z);
	setMaterial(meshId, subentity, newMaterial->getName());
}

Vector3 MeshRenderer::getEmissive(int subentity) const
{
	Ogre::Entity* entity = getMesh(meshId);
	checkNullAndBreak(entity, Vector3::ZERO);
	Ogre::SubEntity* subEntity = entity->getSubEntity(subentity);
	checkNullAndBreak(subEntity, Vector3::ZERO);

	Ogre::MaterialPtr material = subEntity->getMaterial();
	checkNullAndBreak(material.get(), Vector3::ZERO);

	Ogre::Technique* technique = material->getTechnique(0);
	checkNullAndBreak(technique, Vector3::ZERO);
	Ogre::Pass* pass = technique->getPass(0);
	checkNullAndBreak(pass, Vector3::ZERO);

	Ogre::ColourValue c = pass->getEmissive();
	return { c.r, c.g, c.b };
}

int MeshRenderer::getSubentitiesSize() const
{
	Ogre::Entity* entity = getMesh(meshId);
	checkNullAndBreak(entity, 0);

	return entity->getSubEntities().size();
}

void MeshRenderer::setTexture(int subentity, const std::string& textureName)
{
	Ogre::Entity* entity = getMesh(meshId);
	checkNullAndBreak(entity);

	Ogre::SubEntity* subEntity = entity->getSubEntity(subentity);
	checkNullAndBreak(subEntity);

	Ogre::MaterialPtr material = subEntity->getMaterial();
	checkNullAndBreak(material.get());

	Ogre::Technique* technique = material->getTechnique(0);
	checkNullAndBreak(technique);

	Ogre::Pass* pass = technique->getPass(0);
	checkNullAndBreak(pass);

	Ogre::TextureUnitState* texture = pass->getTextureUnitState(0);
	checkNullAndBreak(texture);

	texture->setTextureName(textureName);
}

const std::string& MeshRenderer::getTexture(int subentity) const
{
	Ogre::Entity* entity = getMesh(meshId);
	checkNullAndBreak(entity, "");

	Ogre::SubEntity* subEntity = entity->getSubEntity(subentity);
	checkNullAndBreak(subEntity, "");

	Ogre::MaterialPtr material = subEntity->getMaterial();
	checkNullAndBreak(material.get(), "");

	Ogre::Technique* technique = material->getTechnique(0);
	checkNullAndBreak(technique, "");

	Ogre::Pass* pass = technique->getPass(0);
	checkNullAndBreak(pass, "");

	Ogre::TextureUnitState* texture = pass->getTextureUnitState(0);
	checkNullAndBreak(texture, "");

	return texture->getTextureName();
}

void MeshRenderer::handleData(ComponentData* data)
{
	checkNullAndBreak(data);

	for (auto prop : data->getProperties())
	{
		std::stringstream ss(prop.second);
		if (prop.first == "mesh")
		{
			char c;
			while (ss >> meshId >> meshName)
			{
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
			while (ss >> id >> name >> owner >> bone)
			{
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
				LOG_ERROR("MESH RENDERER", "Wrong value for property %s", prop.first.c_str());
		}
		else if (prop.first == "visible")
		{
			if (ss >> visible)
				setVisible(visible);
			else
				LOG_ERROR("MESH RENDERER", "Wrong value for property %s", prop.first.c_str());
		}
	}
}

Ogre::Entity* MeshRenderer::getMesh(const std::string& mesh) const
{
	if (entities.find(mesh) == entities.end()) {
		LOG_ERROR("MESH RENDERER", "No \"%s\" mesh found", mesh.c_str());
		return nullptr;
	}

	return entities.at(mesh);
}
