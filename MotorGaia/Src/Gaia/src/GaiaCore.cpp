#include "GaiaCore.h"

#include <OgreRoot.h>
#include "Window.h"

#include "EventSystem.h"
#include "RenderSystem.h"
#include "InputSystem.h"
#include "InterfaceSystem.h"
#include "PhysicsSystem.h"
#include "SoundSystem.h"
#include "gTime.h"

#include "ComponentManager.h"
#include "SceneManager.h"

GaiaCore::GaiaCore() :	root(nullptr), window(nullptr),
						eventSystem(nullptr), renderSystem(nullptr), inputSystem(nullptr), 
						interfaceSystem(nullptr), physicsSystem(nullptr), soundSystem(nullptr),
						resourcesManager("resources.asset"), sceneManager(nullptr), componentManager(nullptr)
{

}

GaiaCore::~GaiaCore()
{
	// Call close before GaiaCore destructor
}

void GaiaCore::init()
{
#ifdef _DEBUG
	root = new Ogre::Root("plugins_d.cfg", "window_d.cfg");
#else
	root = new Ogre::Root("plugins.cfg", "window.cfg");
#endif

	if (!(root->restoreConfig() || root->showConfigDialog(nullptr)))
		return;

	// Setup window
	window = new Window(root, "Test window - 2020 (c) Gaia ");

	// Systems initialization
	// EventSystem
	eventSystem = new EventSystem();
	eventSystem->init();
	
	// RenderSystem
	renderSystem = RenderSystem::GetInstance();
	renderSystem->init(root);

	// InputSystem
	inputSystem = InputSystem::GetInstance();
	inputSystem->init();

	// InterfaceSystem
	interfaceSystem = InterfaceSystem::GetInstance();
	interfaceSystem->init(window);

	// PhysicsSystem
	physicsSystem = PhysicsSystem::GetInstance();
	physicsSystem->init();

	// SoundSystem
	soundSystem = SoundSystem::GetInstance();
	soundSystem->init();

	// Managers initialization
	// ResourcesManager initialization
	resourcesManager.init();

	// ComponentManager initialization
	componentManager = ComponentManager::GetInstance();
	componentManager->init();

	//Init Default Resources
	interfaceSystem->initDefaultResources();

	// SceneManager initialization (required ResourcesManager and ComponentManager previous initialization)
	sceneManager = SceneManager::GetInstance();
	sceneManager->init(root, window);

	gTime::GetInstance()->setup();
}

void GaiaCore::run()
{
	float deltaTime = gTime::GetInstance()->getDeltaTime();
	while (!window->isClosed()) {
		// Render
		render(deltaTime);

		// Pre-process
		preUpdate(deltaTime);

		// Process
		update(deltaTime);

		// Post-process
		postUpdate(deltaTime);

		deltaTime = gTime::GetInstance()->getDeltaTime();
	}
}

void GaiaCore::close()
{
	gTime::GetInstance()->destroy();
	// SceneManager termination
	sceneManager->close();
	// ComponentManager termination
	componentManager->close();
	// ResourcesManager termination
	resourcesManager.close();

	//Systems termination
	soundSystem->close();
	physicsSystem->close();
	interfaceSystem->close();
	inputSystem->close();
	renderSystem->close();
	eventSystem->close();

	if (eventSystem != nullptr)
		delete eventSystem;
	eventSystem = nullptr;

	if (window != nullptr)
		delete window;
	window = nullptr;

	if (root != nullptr)
		delete root;
	root = nullptr;
}

void GaiaCore::render(float deltaTime)
{
	// RenderSystem
	renderSystem->render(deltaTime);

#ifdef _DEBUG
	// PhysicsSystem
	physicsSystem->render();
#endif

	// InterfaceSystem
	interfaceSystem->render();
}

void GaiaCore::preUpdate(float deltaTime)
{
	// InputSystem
	inputSystem->preUpdate();

	// EventSystem
	eventSystem->update();

	// InputSystem
	inputSystem->update();

	// InterfaceSystem
	interfaceSystem->update(deltaTime);

	// PhysicsSystem
	physicsSystem->update(deltaTime);

	// SoundSystem
	soundSystem->update(deltaTime);

	// Managers
	sceneManager->preUpdate(deltaTime);
}

void GaiaCore::update(float deltaTime)
{
	sceneManager->update(deltaTime);
}

void GaiaCore::postUpdate(float deltaTime)
{
	sceneManager->postUpdate(deltaTime);

	// Systems 
	physicsSystem->postUpdate();
}