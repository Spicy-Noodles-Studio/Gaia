#pragma once

#include <OgreRoot.h>
#include <OgreRenderWindow.h>

class Window
{
public:
	Window(Ogre::Root* root);
	~Window();

private:
	Ogre::RenderWindow* mWindow;

};

