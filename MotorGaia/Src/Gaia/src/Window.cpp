#include "Window.h"

#include <OgreRenderSystem.h>
#include <iostream>

Window::Window(Ogre::Root* mRoot, Ogre::String cfgFile)
{
	Ogre::RenderSystem* rs = mRoot->getRenderSystemByName("OpenGL Rendering Subsystem");

	mRoot->setRenderSystem(rs);

	// estas opciones deberian ir leidas de archivo
	rs->setConfigOption("Full Screen", "No");
	rs->setConfigOption("Video Mode", "1280 x 720 @ 32-bit colour");
	rs->setConfigOption("FSAA", "16");
	rs->setConfigOption("VSync", "No");
	rs->setConfigOption("VSync Interval", "No");
	rs->setConfigOption("sRGB Gamma Conversion", "No");
	rs->setConfigOption("Colour Depth", "32");
	rs->setConfigOption("RTT Preferred Mode", "FBO");
	
	mWindow = mRoot->initialise(true, "Test Window - (c) Gaia");
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

void Window::setConfigFromFile(Ogre::RenderSystem* rs, Ogre::String file)
{
	 
	//no se muy bien como usar json
	
}

void Window::setFullscreen(bool fullscreen)
{
	mWindow->setFullscreen(fullscreen, 1920, 1080);
}

