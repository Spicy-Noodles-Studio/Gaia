#pragma once

#include <OgreRoot.h>
#include <OgreRenderWindow.h>

class Window
{
public:
	Window(Ogre::Root* root, Ogre::String windowTitle);
	~Window();

	void setFullscreen(bool fullscreen);
	//void setViewport(bool fullscreen);

private:
	Ogre::RenderWindow* mWindow;

	void displayConfig(Ogre::RenderSystem* rs);
};

