#include "GaiaCore.h"

#include <OgreRoot.h>
#include "Window.h"
#include "Timer.h"

#include "EventSystem.h"
#include "RenderSystem.h"
#include "InputSystem.h"
#include "InterfaceSystem.h"
#include "PhysicsSystem.h"
#include "SoundSystem.h"

#include "ComponentManager.h"
#include "SceneManager.h"
#include "WindowManager.h"

#include "Transform.h"
#include "Camera.h"
#include "Light.h"
#include "MeshRenderer.h"
#include "Animator.h"
#include "RigidBody.h"
#include "UILayout.h"
#include "SoundEmitter.h"
#include "SoundListener.h"
#include "Reverb.h"
#include "ParticleEmitter.h"
#include "Strider.h"
#include "Cursor.h"

#define FORCE_LINK_THAT(x) { extern int force_link_##x; force_link_##x = 1; }

GaiaCore::GaiaCore() :	root(nullptr), window(nullptr), timer(nullptr), 
						eventSystem(nullptr), renderSystem(nullptr), inputSystem(nullptr), 
						interfaceSystem(nullptr), physicsSystem(nullptr), soundSystem(nullptr),
						resourcesManager("resources.asset"), sceneManager(nullptr), componentManager(nullptr), windowManager(nullptr)
{

}

GaiaCore::~GaiaCore()
{
	// Call close before GaiaCore destructor
}

void GaiaCore::init(const std::string& windowTitle, unsigned int width, unsigned int height)
{
#ifdef _DEBUG
	root = new Ogre::Root("plugins_d.cfg", "window_d.cfg");
#else
	root = new Ogre::Root("plugins.cfg", "window.cfg", "");
#endif

	checkNullAndBreak(root);
	if (!(root->restoreConfig() || root->showConfigDialog(nullptr)))
		return;

	windowManager = WindowManager::GetInstance();
	checkNullAndBreak(windowManager);
	windowManager->init(root);

	// Setup window
	window = windowManager->createWindow(windowTitle, width, height);

	// Systems initialization
	// EventSystem
	eventSystem = new EventSystem();
	checkNullAndBreak(eventSystem);
	eventSystem->init();

	// RenderSystem
	renderSystem = RenderSystem::GetInstance();
	checkNullAndBreak(renderSystem);
	renderSystem->init(root);

	// InputSystem
	inputSystem = InputSystem::GetInstance();
	checkNullAndBreak(inputSystem);
	inputSystem->init();

	// InterfaceSystem
	interfaceSystem = InterfaceSystem::GetInstance();
	checkNullAndBreak(interfaceSystem);
	interfaceSystem->init(window);

	// PhysicsSystem
	physicsSystem = PhysicsSystem::GetInstance();
	checkNullAndBreak(physicsSystem);
	physicsSystem->init();

	// SoundSystem
	soundSystem = SoundSystem::GetInstance();
	checkNullAndBreak(soundSystem);
	soundSystem->init();

	// Managers initialization
	// ResourcesManager initialization
	resourcesManager.init();

	// ComponentManager initialization
	componentManager = ComponentManager::GetInstance();
	checkNullAndBreak(componentManager);
	componentManager->init();

	// SceneManager initialization (required ResourcesManager and ComponentManager previous initialization)
	sceneManager = SceneManager::GetInstance();
	checkNullAndBreak(sceneManager);
	sceneManager->init(root);

	// Main Engine Timer
	timer = Timer::GetInstance();
	checkNullAndBreak(timer);
	timer->init();
}

void GaiaCore::run()
{
	checkNullAndBreak(timer);
	float deltaTime = timer->getDeltaTime();
	checkNullAndBreak(windowManager);
	while (!windowManager->isClosed())
	{
		// Update elapsed time
		checkNullAndBreak(timer);
		timer->update();
		deltaTime = timer->getDeltaTime();

		// Render
		render(deltaTime);

		// Pre-process
		preUpdate(deltaTime);

		// Process
		update(deltaTime);

		// Post-process
		postUpdate(deltaTime);
	}
}

void GaiaCore::close()
{
	checkNullAndBreak(timer);
	timer->close();

	// SceneManager termination
	checkNullAndBreak(sceneManager);
	sceneManager->close();

	// ComponentManager termination
	checkNullAndBreak(componentManager);
	componentManager->close();

	// ResourcesManager termination
	resourcesManager.close();

	//WindowManager termination
	checkNullAndBreak(windowManager);
	windowManager->close();

	//Systems termination
	checkNullAndBreak(soundSystem);
	soundSystem->close();
	checkNullAndBreak(physicsSystem);
	physicsSystem->close();
	checkNullAndBreak(interfaceSystem);
	interfaceSystem->close();
	checkNullAndBreak(inputSystem);
	inputSystem->close();
	checkNullAndBreak(renderSystem);
	renderSystem->close();
	checkNullAndBreak(eventSystem);
	eventSystem->close();

	if (eventSystem != nullptr)
	{
		delete eventSystem;
		eventSystem = nullptr;
	}

	if (root != nullptr)
	{
		delete root;
		root = nullptr;
	}
}

void GaiaCore::render(float deltaTime)
{
	// RenderSystem
	checkNullAndBreak(renderSystem);
	renderSystem->render(deltaTime);

#ifdef _DEBUG
	// PhysicsSystem
	checkNullAndBreak(physicsSystem);
	physicsSystem->render();
#endif

	// InterfaceSystem
	checkNullAndBreak(interfaceSystem);
	interfaceSystem->render();
}

void GaiaCore::preUpdate(float deltaTime)
{
	// InputSystem
	checkNullAndBreak(inputSystem);
	inputSystem->preUpdate();

	// EventSystem
	checkNullAndBreak(eventSystem);
	eventSystem->update();

	// InputSystem
	checkNullAndBreak(inputSystem);
	inputSystem->update();

	// InterfaceSystem
	checkNullAndBreak(interfaceSystem);
	interfaceSystem->update(deltaTime);

	// PhysicsSystem
	checkNullAndBreak(physicsSystem);
	physicsSystem->update(deltaTime);

	// SoundSystem
	checkNullAndBreak(soundSystem);
	soundSystem->update(deltaTime);

	// Managers
	checkNullAndBreak(sceneManager);
	sceneManager->preUpdate(deltaTime);
}

void GaiaCore::update(float deltaTime)
{
	checkNullAndBreak(sceneManager);
	sceneManager->update(deltaTime);
}

void GaiaCore::postUpdate(float deltaTime)
{
	checkNullAndBreak(sceneManager);
	sceneManager->postUpdate(deltaTime);

	// Systems 
	checkNullAndBreak(inputSystem);
	inputSystem->postUpdate();

	checkNullAndBreak(physicsSystem);
	physicsSystem->postUpdate();
}

//Si exportamos un .lib hacemos que los componentes basicos se exporten siempre
void GaiaCore::initLib()
{
#ifndef USE_DLL
	FORCE_LINK_THAT(Animator);
	FORCE_LINK_THAT(Camera);
	FORCE_LINK_THAT(Light);
	FORCE_LINK_THAT(MeshRenderer);
	FORCE_LINK_THAT(ParticleEmitter);
	FORCE_LINK_THAT(Reverb);
	FORCE_LINK_THAT(RigidBody);
	FORCE_LINK_THAT(SoundEmitter);
	FORCE_LINK_THAT(SoundListener);
	FORCE_LINK_THAT(Strider);
	FORCE_LINK_THAT(Transform);
	FORCE_LINK_THAT(UILayout);
	FORCE_LINK_THAT(Cursor);
#endif
}