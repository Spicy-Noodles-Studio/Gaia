#include "RenderSystem.h"

RenderSystem::RenderSystem() {}

RenderSystem::~RenderSystem() {}

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

Ogre::Entity* RenderSystem::createEntity(std::string mesh)
{
	return sceneManager->createEntity(mesh);
}