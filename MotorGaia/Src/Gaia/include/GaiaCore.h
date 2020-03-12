#pragma once
#ifndef GAIA_CORE_H
#define GAIA_CORE_H

#ifdef GAIA_EXPORTS
#define GAIA_API __declspec(dllexport)
#else
#define GAIA_API __declspec(dllimport)
#endif

#include <OgreRoot.h>
#include "Window.h"

#include "RenderSystem.h"
#include "InputSystem.h"
#include "InterfaceSystem.h"
#include "PhysicsSystem.h"
#include "SoundSystem.h"

#include "ResourcesManager.h"
#include "ComponentManager.h"
#include "SceneManager.h"

#include "gTime.h"

class GAIA_API GaiaCore
{
public:
	GaiaCore();
	~GaiaCore();

	void init();
	void run();
	void close();

private:
	void render(float deltaTime);
	void preUpdate(float deltaTime);
	void update(float deltaTime);
	void postUpdate(float deltaTime);

	Ogre::Root* root;
	Window* win;

	//Systems
	RenderSystem* renderSystem;
	InputSystem* inputSystem;
	InterfaceSystem* interfaceSystem;
	PhysicsSystem* physicsSystem;
	SoundSystem* soundSystem;

	//Managers
	ResourcesManager resourcesManager;
	ComponentManager* componentManager;
	SceneManager* sceneManager;
};

#endif 