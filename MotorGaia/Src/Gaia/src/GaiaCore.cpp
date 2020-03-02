#include "GaiaCore.h"
//Ogre includes
#include <OgreRoot.h>
#include <OgreException.h>
#include <OgreConfigFile.h>
#include <iostream>

#include "RenderSystem.h"
#include "Window.h"
#include "Camera.h"
#include "GameObject.h"
#include "OgreViewport.h"

#include "RenderComponent.h"

GaiaCore::GaiaCore()
{

}

GaiaCore::~GaiaCore()
{
	delete mRoot;
}

void GaiaCore::setupResources()
{
	// Ogre configuration loader
	Ogre::ConfigFile cf;
	cf.load(mResourcesCfg);

	Ogre::String name, locationType;
	Ogre::ConfigFile::SettingsBySection_ settingsBySection = cf.getSettingsBySection();
	for (const auto& p : settingsBySection) {
		for (const auto& r : p.second) {
			locationType = r.first;
			name = r.second;
			Ogre::ResourceGroupManager::getSingleton().addResourceLocation(name, locationType);
		}
	}

	
}

void GaiaCore::init()
{
#ifdef _DEBUG
	mResourcesCfg = "resources_d.cfg";
	mPluginsCfg = "plugins_d.cfg";
	mWindowCfg = "window_d.cfg";
#else
	mResourcesCfg = "resources.cfg";
	mPluginsCfg = "plugins.cfg";
	mWindowCfg = "window.cfg";
#endif

	// Ogre initialization
	mRoot = new Ogre::Root(mPluginsCfg, mWindowCfg);
	setupResources();

	if (!mRoot->restoreConfig());

	// Setup window
	Window* win = new Window(mRoot, "Test window - 2020 (c) Gaia ");

	Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

	RenderSystem::GetInstance()->setup(mRoot);

	GameObject* go = new GameObject("Camera", "Cam", nullptr);
	Camera* cam = new Camera(go);

	Ogre::Viewport* vp = win->addViewport(cam->getCamera());

	RenderComponent* rc = new RenderComponent(go);
	rc->createEntity("knot", "knot.mesh");
	rc->getNode()->setPosition({ 0,0,-400 });

	RenderSystem::GetInstance()->getSceneManager()->setAmbientLight(Ogre::ColourValue(.5, .5, .5));
	Ogre::Light* light = RenderSystem::GetInstance()->getSceneManager()->createLight("MainLight");

}

void GaiaCore::run()
{
	while (true)
	{
		update();
	}
}

void GaiaCore::update()
{
	mRoot->renderOneFrame();
}