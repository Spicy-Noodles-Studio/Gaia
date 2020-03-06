#pragma once

#include <OgreRoot.h>
#include "Singleton.h"

#include <CEGUI/CEGUI.h>

class InterfaceSystem : public Singleton<InterfaceSystem>
{
private:
	Ogre::Root* root;

	CEGUI::Window* myRoot;

public:
	InterfaceSystem();
	~InterfaceSystem();

	void setup(Ogre::Root* _root);
	void render();
};

