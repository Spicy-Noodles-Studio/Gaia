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

GaiaCore::GaiaCore() : root(nullptr), win(nullptr), resourcesManager("resources.asset"), obj(nullptr), count(0)
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

	win = new Window(root, "Ventana de Prueba");

	// ResourcesManager initialization
	resourcesManager.init();
	// SceneManager initialization (required ResourcesManager previous initialization)
	sceneManager.init(root, win);
	//Pruebas
	RenderSystem::GetInstance()->init(root);

	//REGISTRO DE COMPONENTES (probablemente se deberia de pasar al init de componentManager)
	componentManager.registerComponent<Transform>("Transform");
	componentManager.registerComponent<Camera>("Camera");
	componentManager.registerComponent<Light>("Light");
	componentManager.registerComponent<MeshRenderer>("MeshRenderer");

	// Carga de escena
	sceneManager.changeScene("MainScene");
	obj = sceneManager.getCurrentScene()->findGameObjectWithName("Nudo");	
}

void GaiaCore::run()
{
	bool exit = false;
	float deltaTime = 1.f / 60.f;
	while (!exit) {
		// Render
		RenderSystem::GetInstance()->render(deltaTime);

		// Stuff like update
		preUpdate(deltaTime);
		update(deltaTime);
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

void GaiaCore::preUpdate(float deltaTime)
{
	// Systems

	// Managers
	sceneManager.preUpdate(deltaTime);
}

void GaiaCore::update(float deltaTime)
{
	// Systems

	// Managers
	sceneManager.update(deltaTime);

	//Eliminar
	if (++count >= 10000) // 10 segundos
		sceneManager.changeScene("Scene1");
}

void GaiaCore::postUpdate(float deltaTime)
{
	// Systems 

	// Managers
	sceneManager.postUpdate(deltaTime);
}
