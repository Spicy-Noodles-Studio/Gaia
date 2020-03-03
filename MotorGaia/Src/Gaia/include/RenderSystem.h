#ifndef RENDER_SYSTEM
#define RENDER_SYSTEM

#pragma once
#include <OgreRoot.h>
#include <OgreSceneManager.h>
#include <OgreSceneNode.h>
#include <OgreEntity.h>

#include "Singleton.h"

class RenderSystem : public Singleton<RenderSystem>
{
private:
	Ogre::SceneManager* sceneManager;

public:
	RenderSystem();

	void setup(Ogre::Root* root);
	void shutDown(Ogre::Root* root);

	Ogre::SceneManager* getSceneManager();

	Ogre::Entity* createEntity(Ogre::Entity* entity, std::string mesh);
	void setMaterial(Ogre::Entity* entity, std::string material);
	Ogre::SceneNode* createChildNode(Ogre::SceneNode* node, Ogre::Entity* entity, std::string id);

};

#endif