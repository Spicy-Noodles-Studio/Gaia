#pragma once

#ifndef WINDOW_H
#define WINDOW_H

#include <OgreRoot.h>
#include <OgreRenderWindow.h>

class Window
{
public:
	Window(Ogre::Root* root, std::string windowTitle);
	~Window();

	void setFullscreen(bool fullscreen);
	//void setViewport(bool fullscreen);

private:
	Ogre::RenderWindow* mWindow;

	void displayConfig(Ogre::RenderSystem* rs);
};

#endif