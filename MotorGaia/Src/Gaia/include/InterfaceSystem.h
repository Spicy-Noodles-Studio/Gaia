#pragma once

#include <OgreRoot.h>
#include "Singleton.h"

#include <CEGUI/CEGUI.h>
#include <CEGUI/Renderer.h>
#include <CEGUI/RendererModules/Ogre/Renderer.h>

#include "Window.h"

class InterfaceSystem : public Singleton<InterfaceSystem>
{
private:
	Ogre::Root* root;

	CEGUI::OgreRenderer* mRenderer;

	CEGUI::Window* myRoot;


	bool clicked(const CEGUI::EventArgs& args);


public:
	InterfaceSystem();
	~InterfaceSystem();

	void setup(Ogre::Root* _root, Window* window);
	void render();
	void update(float deltaTime);
};

