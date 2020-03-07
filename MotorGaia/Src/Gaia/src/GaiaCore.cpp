#include "GaiaCore.h"

#include <OgreRoot.h>
#include <OgreException.h>
#include <OgreConfigFile.h>
#include <OgreViewport.h>
#include <iostream>
// Example
#include "RenderSystem.h"
#include "Camera.h"
#include "Light.h"
#include "GameObject.h"
#include "Transform.h"
#include "MeshRenderer.h"

GaiaCore::GaiaCore() : root(nullptr), win(nullptr), resourcesManager("resources.asset"), obj(nullptr)
{

}

GaiaCore::~GaiaCore()
{
	delete root;
	delete obj;
    delete win;
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

	// Managers initialization
	// ResourcesManager initialization
	resourcesManager.init();

	// TODO: ComponentManager initialization
	// componentManager.init();

	//REGISTRO DE COMPONENTES (probablemente se deberia de pasar al init de componentManager)
	componentManager.registerComponent<Transform>("Transform");
	componentManager.registerComponent<Camera>("Camera");
	componentManager.registerComponent<Light>("Light");
	componentManager.registerComponent<MeshRenderer>("MeshRenderer");

	// SceneManager initialization (required ResourcesManager and ComponentManager previous initialization)
	sceneManager.init(root, win);

}

void GaiaCore::run()
{
	bool exit = false;
	float deltaTime = 1.f / 60.f;
	while (!exit) {
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
	sceneManager.close();
	// ResourcesManager termination
	resourcesManager.close();
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
	// InputSystem::GetInstance()->update(deltaTime);


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
