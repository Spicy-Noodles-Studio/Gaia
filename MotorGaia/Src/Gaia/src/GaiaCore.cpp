#include "GaiaCore.h"

#include <OgreRoot.h>
#include <OgreException.h>
#include <OgreConfigFile.h>
#include <OgreViewport.h>
#include <iostream>

#include "ResourcesManager.h"
#include "RenderSystem.h"
#include "Window.h"
#include "Camera.h"
#include "Light.h"

#include "GameObject.h"
#include "Transform.h"
#include "MeshRenderer.h"

GaiaCore::GaiaCore()
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

	// Setup window
	Window* win = new Window(root, "Test window - 2020 (c) Gaia ");

	ResourcesManager rManager("resources.asset");
	rManager.init();

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
	ms->attachEntityToNode("knot");
	obj->transform->setPosition(Vector3(0, 0, -400));
	obj->transform->setScale(Vector3(0.5, 0.5, 0.5));
	obj->transform->rotate(Vector3(0, 90, 0));
}

void GaiaCore::run()
{
	while (true)
	{
		RenderSystem::GetInstance()->render();
		update();
	}
}

void GaiaCore::close()
{

}

void GaiaCore::update()
{
	obj->transform->translate(Vector3(0.5, 0, 0));
}