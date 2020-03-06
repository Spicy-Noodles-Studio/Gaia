#include "RenderSystem.h"

RenderSystem::RenderSystem()
{

}

RenderSystem::~RenderSystem()
{

}

void RenderSystem::setup(Ogre::Root* _root)
{
	root = _root;
	sceneManager = _root->createSceneManager();
}

void RenderSystem::render()
{
	root->renderOneFrame();
}

void RenderSystem::shutDown()
{
	if (root != nullptr)
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