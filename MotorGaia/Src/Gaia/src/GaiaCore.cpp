#include "GaiaCore.h"

#include <OgreRoot.h>
#include <OgreException.h>
#include <OgreConfigFile.h>
#include <OgreViewport.h>
#include <iostream>

#include "RenderSystem.h"
#include "GaiaInput.h"

GaiaCore::GaiaCore() : root(nullptr), win(nullptr), resourcesManager("resources.asset")
{

}

GaiaCore::~GaiaCore()
{
	close();
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
	RenderSystem::GetInstance()->init(root);
	GaiaInput::GetInstance()->init();

	// Managers initialization
	// ResourcesManager initialization
	resourcesManager.init();

	// ComponentManager initialization
	componentManager.init();

	// SceneManager initialization (required ResourcesManager and ComponentManager previous initialization)
	sceneManager.init(root, win);

	GameObject* obj = sceneManager.getCurrentScene()->getGameObjectWithName("Nudo 1");
	component = new UserComponent(obj);
}

void GaiaCore::run()
{
	bool exit = false;
	float deltaTime = 1.f / 60.f;
	while (!GaiaInput::GetInstance()->getKeyPress("Escape")) {
		// Render
		render(deltaTime);

		// Pre-process
		preUpdate(deltaTime);
		
		// Process
		update(deltaTime);

		//PRUEBAS
		if (GaiaInput::GetInstance()->getKeyPress("Space")) {
			double x = (rand() % 200) + 1;
			double y = (rand() % 200) + 1;
			Vector3 pos = { x, y, -400 };
			component->instantiate("Nudo", pos);
		}
		if (GaiaInput::GetInstance()->getKeyPress("d")) {
			auto nudos = component->findGameObjectsWithTag("nudo");
			for (auto n : nudos)
				component->destroy(n);
		}

		// Post-process
		postUpdate(deltaTime);
	}
}

void GaiaCore::close()
{
	// SceneManager termination
	sceneManager.close();
	// ComponentManager termination
	componentManager.close();
	// ResourcesManager termination
	resourcesManager.close();

	//Systems termination
	GaiaInput::destroy();
	RenderSystem::destroy();

	if (win != nullptr)
		delete win;
	win = nullptr;
	if (root != nullptr)
		delete root;
	root = nullptr;
	if (component != nullptr)
		delete component;
	component = nullptr;
}

void GaiaCore::render(float deltaTime)
{
	// RenderSystem
	RenderSystem::GetInstance()->render(deltaTime);
	// InterfaceSystem
	// InterfaceSystem::GetInstance()->render(deltaTime);
}

void GaiaCore::preUpdate(float deltaTime)
{
	// Systems TODO:
	// RenderSystem (animations)
	// RenderSystem::GetInstance()->update(deltaTime);

	// InterfaceSystem
	// InterfaceSystem::GetInstance()->update(deltaTime);
	
	// InputSystem
	GaiaInput::GetInstance()->update();


	// Managers
	sceneManager.preUpdate(deltaTime);
}

void GaiaCore::update(float deltaTime)
{
	// Managers
	sceneManager.update(deltaTime);
}

void GaiaCore::postUpdate(float deltaTime)
{
	// Managers
	sceneManager.postUpdate(deltaTime);

	// Systems 
	// Si es que hay
}
