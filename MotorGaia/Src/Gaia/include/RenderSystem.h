#pragma once
#ifndef RENDER_SYSTEM_H
#define RENDER_SYSTEM_H

#include <OgreRoot.h>
#include <OgreSceneManager.h>
#include <OgreEntity.h>

#include "Singleton.h"

class GAIA_API RenderSystem : public Singleton<RenderSystem>
{
private:
	Ogre::Root* root;

public:
	RenderSystem();
	~RenderSystem();

	void init(Ogre::Root* root);
	void render(float deltaTime);
	void close();
};

#endif