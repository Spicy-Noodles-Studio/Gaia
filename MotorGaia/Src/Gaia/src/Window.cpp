#include "Window.h"

#include <OgreRenderSystem.h>
#include <iostream>

#include <SDL2/SDL.h>
#include <SDL2/SDL_video.h>
#include <SDL2/SDL_syswm.h>
//#include <SDL2/SDLInputMapping.h>

Window::Window(Ogre::Root* mRoot, std::string windowTitle)
{
	Ogre::RenderSystem* rs = mRoot->getRenderSystemByName("OpenGL Rendering Subsystem");
	mRoot->setRenderSystem(rs);
	
	//mWindow = mRoot->initialise(false, windowTitle);

	if (!SDL_WasInit(SDL_INIT_VIDEO)) SDL_InitSubSystem(SDL_INIT_VIDEO);

	Uint32 flags = SDL_WINDOW_RESIZABLE;

	//if (ropts["Full Screen"].currentValue == "Yes")  flags = SDL_WINDOW_FULLSCREEN;
	//else  flags = SDL_WINDOW_RESIZABLE;

	SDL_Window* sdlWin = SDL_CreateWindow(windowTitle.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1024, 768, flags);

	mWindow = (Ogre::RenderWindow*)mRoot->getRenderTarget(windowTitle);

	/*HWND hwnd;
	mWindow->getCustomAttribute("WINDOW", &hwnd);
	ShowWindow((HWND)hwnd, SW_HIDE);*/

	

	

	/*SDL_SysWMinfo wmInfo;
	SDL_VERSION(&wmInfo.version);
	SDL_GetWindowWMInfo(mWindow.native, &wmInfo);

	miscParams["externalWindowHandle"] = Ogre::StringConverter::toString(size_t(wmInfo.info.win.window));

	mWindow.render = mRoot->createRenderWindow(name, w, h, false, &miscParams);*/

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

void Window::addViewport(Ogre::Camera* cam)
{
	mWindow->addViewport(cam);
}

void Window::setFullscreen(bool fullscreen)
{
	mWindow->setFullscreen(fullscreen, 1920, 1080);
}

void Window::setFSAA(unsigned int fsaa)
{
	mWindow->setFSAA(fsaa, "");
}

