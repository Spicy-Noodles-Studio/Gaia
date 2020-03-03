#include "GaiaCore.h"
#include <OgreRoot.h>

#include <iostream>

GaiaCore::GaiaCore() : win(nullptr), resourcesManager("resources.asset")
{

}

GaiaCore::~GaiaCore()
{
	delete win;
}

void GaiaCore::init()
{
#ifdef _DEBUG
	Ogre::Root* r = new Ogre::Root("plugins_d.cfg", "window_d.cfg");
#else
	Ogre::Root* r = new Ogre::Root("plugins.cfg", "window.cfg");
#endif

	if (!(r->restoreConfig() || r->showConfigDialog(nullptr)))
		return;

	win = new Window(r, "Ventana de Prueba");

	// ResourcesManager initialization
	resourcesManager.init();
	// SceneManager initialization (required ResourcesManager previous initialization)
	sceneManager.init(r);

	//Pruebas

	sceneManager.changeScene("MainScene");

}

void GaiaCore::run()
{
	bool exit = false;
	float deltaTime = 16.6;
	while (!exit) {
		//Stuff like render

		//Stuff like update
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
	//Systems

	//Managers
	sceneManager.preUpdate(deltaTime);
}

void GaiaCore::update(float deltaTime)
{
	// Systems

	// Managers
	sceneManager.update(deltaTime);
}

void GaiaCore::postUpdate(float deltaTime)
{
	// Systems 

	// Managers
	sceneManager.postUpdate(deltaTime);
}
