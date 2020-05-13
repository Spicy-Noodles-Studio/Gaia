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

void GaiaCore::init(std::string windowName)
{
#ifdef _DEBUG
	root = new Ogre::Root("plugins_d.cfg", "window_d.cfg");
#else
	root = new Ogre::Root("plugins.cfg", "window.cfg");
#endif

	if (!(root->restoreConfig() || root->showConfigDialog(nullptr)))
		return;

	windowManager = new WindowManager();
	windowManager->createWindow(root, windowName);

	// Setup window
	window = windowManager->getWindow();
	windowManager->initResolutions();

	std::vector<std::pair<int, int>> resolutions = windowManager->getAvailableResolutionsForWindow();

	windowManager->setWindowMinArea(resolutions[0].first, resolutions[0].second);
	windowManager->windowResize(resolutions[resolutions.size() / 2].first, resolutions[resolutions.size() / 2].second);
	windowManager->setActualResolutionId(resolutions.size() / 2);

	// Systems initialization

	// EventSystem
	eventSystem = new EventSystem();
	eventSystem->init();

	// RenderSystem
	renderSystem = RenderSystem::GetInstance();
	renderSystem->init(root, window);

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
	interfaceSystem->initDefaultResources("./Assets/Files/interface.asset");

	// SceneManager initialization (required ResourcesManager and ComponentManager previous initialization)
	sceneManager = SceneManager::GetInstance();
	sceneManager->init(root);

	// Main Engine Timer
	timer = Timer::GetInstance();
	timer->init();
}

void GaiaCore::run()
{
	float deltaTime = timer->getDeltaTime();
	while (!windowManager->isWindowClosed())
	{
		// Update elapsed time
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
	Timer::GetInstance()->close();

	// SceneManager termination
	sceneManager->close();

	// ComponentManager termination
	componentManager->close();

	// ResourcesManager termination
	resourcesManager.close();

	//WindowManager termination
	windowManager->close();

	//Systems termination
	soundSystem->close();
	physicsSystem->close();
	interfaceSystem->close();
	inputSystem->close();
	renderSystem->close();
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
#endif
}