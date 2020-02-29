#pragma once

#include <OgreRoot.h>
#include <OgreRenderWindow.h>

class Window
{
public:
	Window(Ogre::Root* root, Ogre::String cfgFile);
	~Window();

	void setFullscreen(bool fullscreen);
	//void setViewport(bool fullscreen);

private:
	Ogre::RenderWindow* mWindow;

	void displayConfig(Ogre::RenderSystem* rs);
	void setConfigFromFile(Ogre::RenderSystem* rs, Ogre::String file);
};

