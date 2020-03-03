#pragma once

#ifndef GAIA_CORE_H
#define GAIA_CORE_H

#ifdef GAIA_EXPORTS
#define GAIA_API __declspec(dllexport)
#else
#define GAIA_API __declspec(dllimport)
#endif

#include "Window.h"
#include "ResourcesManager.h"
#include "SceneManager.h"

class GAIA_API GaiaCore
{
public:
	GaiaCore();
	~GaiaCore();

	void init();
	void run();
	void close();

private:
	void preUpdate(float deltaTime);
	void update(float deltaTime);
	void postUpdate(float deltaTime);

	Window* win;
	ResourcesManager resourcesManager;
	SceneManager sceneManager;
};

#endif 