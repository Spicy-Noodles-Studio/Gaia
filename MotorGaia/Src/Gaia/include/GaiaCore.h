#pragma once
#ifndef GAIA_CORE_H
#define GAIA_CORE_H

#include "ResourcesManager.h"
#include "GaiaExports.h"

namespace Ogre {
	class Root;
}

class Window;

class EventSystem;
class RenderSystem;
class InputSystem;
class PhysicsSystem;
class InterfaceSystem;
class SoundSystem;

class ComponentManager;
class SceneManager;


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
	Window* window;

	//Systems
	EventSystem* eventSystem;
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