#pragma once
#ifndef RENDER_SYSTEM_H
#define RENDER_SYSTEM_H

#include <OgreRoot.h>
#include <OgreSceneManager.h>
#include <OgreEntity.h>
class Window;

#include "Singleton.h"

class Window;

class GAIA_API RenderSystem : public Singleton<RenderSystem>
{
private:
	Ogre::Root* root;
	Window* window;
public:
	RenderSystem();
	~RenderSystem();

	void init(Ogre::Root* root, Window* window);
	void render(float deltaTime);
	void close();

	void changeParamOfShader(const std::string& material, const std::string& paramName, float paramValue);
	void applyBrightness(Ogre::Viewport* vp);

};

#endif