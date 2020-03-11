#include "Window.h"

#include <OgreRenderSystem.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_video.h>
#include <SDL2/SDL_syswm.h>

#include <iostream>

Window::Window(Ogre::Root* root, std::string windowTitle) : root(root)
{
	Ogre::RenderSystem* rs = root->getRenderSystem();//ByName("OpenGL Rendering Subsystem");
	root->setRenderSystem(rs);

	root->initialise(false);

	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		OGRE_EXCEPT(Ogre::Exception::ERR_INTERNAL_ERROR, "Cannot initialize SDL2!",
			"BaseApplication::setup");
	}

	SDL_ShowCursor(SDL_DISABLE);

	Ogre::ConfigOptionMap ropts = rs->getConfigOptions();
	std::istringstream mode(ropts["Video Mode"].currentValue);
	Ogre::String token;
	int w, h;
	mode >> w; // width
	mode >> token; // 'x' separator
	mode >> h; // height

	Uint32 flags = SDL_WINDOW_RESIZABLE;
	sdlWindow = SDL_CreateWindow(windowTitle.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, w, h, flags);

	SDL_SysWMinfo wmInfo;
	SDL_VERSION(&wmInfo.version);
	if (SDL_GetWindowWMInfo(sdlWindow, &wmInfo) == SDL_FALSE) {
		OGRE_EXCEPT(Ogre::Exception::ERR_INTERNAL_ERROR,
			"Couldn't get WM Info! (SDL2)",
			"BaseApplication::setup");
	}

	Ogre::NameValuePairList params;

	params["FSAA"] = ropts["FSAA"].currentValue;
	params["vsync"] = ropts["VSync"].currentValue;
	params["gamma"] = ropts["sRGB Gamma Conversion"].currentValue;

	params["externalWindowHandle"] = Ogre::StringConverter::toString(size_t(wmInfo.info.win.window));

	mWindow = root->createRenderWindow(windowTitle.c_str(), w, h, false, &params);
}

Window::~Window()
{
	root->destroyRenderTarget(mWindow);
	SDL_DestroyWindow(sdlWindow);
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

void Window::removeAllViewports()
{
	mWindow->removeAllViewports();
}

void Window::setFullscreen(bool fullscreen)
{
	mWindow->setFullscreen(fullscreen, mWindow->getWidth(), mWindow->getHeight());
	//mWindow->update();
}

void Window::setFSAA(unsigned int fsaa)
{
	mWindow->setFSAA(fsaa, "");
}
