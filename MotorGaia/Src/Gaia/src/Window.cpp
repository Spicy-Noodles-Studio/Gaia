#include "Window.h"

#include <OgreRenderSystem.h>
#include <iostream>

Window::Window(Ogre::Root* mRoot, std::string windowTitle)
{
	Ogre::RenderSystem* rs = mRoot->getRenderSystemByName("OpenGL Rendering Subsystem");
	mRoot->setRenderSystem(rs);
	
	mWindow = mRoot->initialise(true, windowTitle);
}

Window::~Window()
{

}

void Window::displayConfig(Ogre::RenderSystem* rs)
{
	Ogre::ConfigOptionMap m = rs->getConfigOptions();
	for (auto it = m.begin(); it != m.end(); it++)
	{
		std::cout << "Name: " << it->second.name << "\n";
		std::cout << "Values: " << "\n";

		for (int i = 0; i < it->second.possibleValues.size(); i++)
		{
			std::cout << it->second.possibleValues.at(i) << ", ";
		}
	}
}

Ogre::Viewport* Window::addViewport(Ogre::Camera* cam)
{
	return mWindow->addViewport(cam);
}

void Window::setFullscreen(bool fullscreen)
{
	mWindow->setFullscreen(fullscreen, 1920, 1080);
}

void Window::setFSAA(unsigned int fsaa)
{
	mWindow->setFSAA(fsaa, "");
}
