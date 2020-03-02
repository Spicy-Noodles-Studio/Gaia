#pragma once

#include <OgreRoot.h>
#include <OgreRenderWindow.h>

class Window
{
public:
	Window(Ogre::Root* root, std::string windowTitle);
	~Window();

	void addViewport(Ogre::Camera* cam);

	void setFullscreen(bool fullscreen);
	void setFSAA(unsigned int fsaa);

private:
	Ogre::RenderWindow* mWindow;

	void displayConfig(Ogre::RenderSystem* rs);
};

