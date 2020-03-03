#pragma once
#ifndef RENDER_SYSTEM_H
#define RENDER_SYSTEM_H

#include <OgreRoot.h>
#include <OgreSceneManager.h>
#include <OgreEntity.h>

#ifndef RENDER_SYSTEM_H
#define RENDER_SYSTEM_H

#include "Singleton.h"

class RenderSystem : public Singleton<RenderSystem>
{
private:
	Ogre::Root* root;
	Ogre::SceneManager* sceneManager;

public:
	RenderSystem();
	~RenderSystem();

	void setup(Ogre::Root* _root);
	void render();
	void shutDown();

	Ogre::SceneManager* getSceneManager();
	Ogre::Entity* createEntity(std::string mesh);
};

#endif