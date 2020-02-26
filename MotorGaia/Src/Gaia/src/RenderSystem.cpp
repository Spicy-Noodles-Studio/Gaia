#include "RenderSystem.h"

RenderSystem::RenderSystem()
{

}

void RenderSystem::setup(Ogre::Root* root)
{
	sceneManager = root->createSceneManager();
}

void RenderSystem::shutDown(Ogre::Root* root)
{
	root->destroySceneManager(sceneManager);
}

Ogre::SceneManager* RenderSystem::getSceneManager()
{
	return sceneManager;
}

Ogre::Entity* RenderSystem::createEntity(Ogre::Entity* entity, std::string mesh)
{
	return RenderSystem::GetInstance()->getSceneManager()->createEntity(mesh);
}

void RenderSystem::setMaterial(Ogre::Entity* entity, std::string material)
{
	entity->setMaterialName(material);
}

Ogre::SceneNode* RenderSystem::createChildNode(Ogre::SceneNode* node, Ogre::Entity* entity, std::string id)
{
	Ogre::SceneNode* child = node->createChildSceneNode(id);
	child->attachObject(entity);

	return child;
}