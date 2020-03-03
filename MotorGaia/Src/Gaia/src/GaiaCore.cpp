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

GaiaCore::GaiaCore() : win(nullptr), resourcesManager("resources.asset")
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
	sceneManager.init(root);


	//Pruebas
	sceneManager.changeScene("MainScene");
	
	RenderSystem::GetInstance()->setup(root);

	GameObject* aux = new GameObject("Camera", "Cam", nullptr);
	Transform* transform1 = new Transform(aux);
	Camera* cam = new Camera(aux);

	Ogre::Viewport* vp = win->addViewport(cam->getCamera());

	Light* lz = new Light(aux);
	lz->setType(Light::Point);
	lz->setColour(0.7, 0.1, 0.7);

	obj = new GameObject("Churro", "Ch", nullptr);
	Transform* transform2 = new Transform(obj);
	MeshRenderer* ms = new MeshRenderer(obj);
	ms->createEntity("knot", "knot.mesh");
	obj->transform->setPosition(Vector3(0, 0, -400));
	obj->transform->setScale(Vector3(0.5, 0.5, 0.5));
	obj->transform->rotate(Vector3(0, 90, 0));
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

	//ELIMINAR
	obj->transform->translate(Vector3(0.5, 0, 0));
}

void GaiaCore::postUpdate(float deltaTime)
{
	// Systems 

	// Managers
	sceneManager.postUpdate(deltaTime);
}
