#include "GaiaCore.h"


GaiaCore::GaiaCore() :	root(nullptr), win(nullptr), 
						renderSystem(nullptr), inputSystem(nullptr),
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

	// Window initialization
	win = new Window(root, "Ventana de Prueba");

	// Systems initialization
	// RenderSystem
	renderSystem = RenderSystem::GetInstance();
	renderSystem->init(root);

	//InputSystem
	inputSystem = InputSystem::GetInstance();
	inputSystem->init();

	// Managers initialization
	// ResourcesManager initialization
	resourcesManager.init();

	// ComponentManager initialization
	componentManager = ComponentManager::GetInstance();
	componentManager->init();

	// SceneManager initialization (required ResourcesManager and ComponentManager previous initialization)
	sceneManager = SceneManager::GetInstance();
	sceneManager->init(root, win);

}

void GaiaCore::run()
{
	bool exit = false;
	float deltaTime = 1.f / 60.f;
	while (!inputSystem->getKeyPress("Escape")) {
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
	// SceneManager termination
	sceneManager->close();
	// ComponentManager termination
	componentManager->close();
	// ResourcesManager termination
	resourcesManager.close();

	//Systems termination
	inputSystem->close();
	renderSystem->close();

	if (win != nullptr)
		delete win;
	win = nullptr;
	if (root != nullptr)
		delete root;
	root = nullptr;
}

void GaiaCore::render(float deltaTime)
{
	// RenderSystem
	renderSystem->render(deltaTime);
	// InterfaceSystem
	// InterfaceSystem::GetInstance()->render(deltaTime);
}

void GaiaCore::preUpdate(float deltaTime)
{
	// Systems TODO:
	// RenderSystem (animations)
	// renderSystem->update(deltaTime);

	// InterfaceSystem
	// InterfaceSystem::GetInstance()->update(deltaTime);
	
	// InputSystem
	inputSystem->update();

	// Managers
	sceneManager->preUpdate(deltaTime);
}

void GaiaCore::update(float deltaTime)
{
	// Managers
	sceneManager->update(deltaTime);
}

void GaiaCore::postUpdate(float deltaTime)
{
	// Managers
	sceneManager->postUpdate(deltaTime);

	// Systems 
	// Si es que hay
}
