#pragma once
#ifndef RENDER_SYSTEM_H
#define RENDER_SYSTEM_H

#include "Singleton.h"
#include <string>

namespace Ogre {
	class Root;
	class Viewport;
}

class Window;

class GAIA_API RenderSystem : public Singleton<RenderSystem>
{
	friend class GaiaCore;
	friend class SceneManager;
private:
	Ogre::Root* root;

public:
	RenderSystem();
	~RenderSystem();

	void changeParamOfShader(const std::string& material, const std::string& paramName, float paramValue);

private:
	void init(Ogre::Root* root);
	void render(float deltaTime);
	void close();

	void applyBrightness(Ogre::Viewport* viewport);
};

#endif