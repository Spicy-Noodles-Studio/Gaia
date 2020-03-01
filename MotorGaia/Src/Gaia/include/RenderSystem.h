#ifndef RENDER_SYSTEM_H
#define RENDER_SYSTEM_H

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
	~RenderSystem();

	void setup(Ogre::Root* root);
	void shutDown(Ogre::Root* root);

	Ogre::SceneManager* getSceneManager();

	Ogre::Entity* createEntity(std::string mesh);
};

#endif