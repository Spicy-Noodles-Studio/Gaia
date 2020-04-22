#pragma once
#ifndef RENDER_SYSTEM_H
#define RENDER_SYSTEM_H

#include <OgreRoot.h>
#include <OgreSceneManager.h>
#include <OgreEntity.h>
class Window;

#include "Singleton.h"

class GAIA_API RenderSystem : public Singleton<RenderSystem>
{
private:
	Ogre::Root* root;
	Window* window;
public:
	RenderSystem();
	~RenderSystem();

	void init(Ogre::Root* root,Window* window_);
	void render(float deltaTime);
	void close();

	void changeParamOfShader(const std::string& material, const std::string& paramName, float paramValue);
	void windowResize(unsigned int width, unsigned int height);
	void setFullscreen(bool fullscreen);
	void ApplyBrightnessToVp(Ogre::Viewport* vp);
	void closeWindow();
};

#endif