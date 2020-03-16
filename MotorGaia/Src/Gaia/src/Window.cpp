#include "Window.h"

#include <OgreRenderSystem.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_video.h>
#include <SDL2/SDL_syswm.h>
#include <OgreViewport.h>

#include <iostream>

Window::Window(Ogre::Root* root, std::string windowTitle) : root(root), closed(false)
{
	Ogre::RenderSystem* rs = root->getRenderSystem();//ByName("OpenGL Rendering Subsystem");
	root->setRenderSystem(rs);

	root->initialise(false);

	SDL_SetMainReady();
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		OGRE_EXCEPT(Ogre::Exception::ERR_INTERNAL_ERROR, "Cannot initialize SDL2!",
			"BaseApplication::setup");
	}

	//SDL_ShowCursor(SDL_DISABLE);

	Ogre::ConfigOptionMap ropts = rs->getConfigOptions();
	std::istringstream mode(ropts["Video Mode"].currentValue);
	Ogre::String token;
	int w, h;
	mode >> w; // width
	mode >> token; // 'x' separator
	mode >> h; // height

	Uint32 flags = SDL_WINDOW_RESIZABLE;
	sdlWindow = SDL_CreateWindow(windowTitle.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, w, h, flags);

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

	window = root->createRenderWindow(windowTitle.c_str(), w, h, false, &params);

	this->width = w;
	this->height = h;
	SDL_GetWindowPosition(sdlWindow, &xPosition, &yPosition);

	// Callbacks definition
	onWindowResized([this](unsigned int width, unsigned int height) { resized(width, height); });
	onWindowMoved([this](int x, int y) { moved(x, y); });
	onWindowClose([this]() { close(); });
}

Window::~Window()
{
	//root->destroyRenderTarget(window);
	SDL_DestroyWindow(sdlWindow);
}


Ogre::Viewport* Window::addViewport(Ogre::Camera* cam)
{
	return window->addViewport(cam);
}

RenderWindow* Window::getRenderWindow()
{
	return window;
}

void Window::removeAllViewports()
{
	window->removeAllViewports();
}

void Window::setFullscreen(bool fullscreen)
{
	window->setFullscreen(fullscreen, window->getWidth(), window->getHeight());
	//mWindow->update();
}

void Window::setFSAA(unsigned int fsaa)
{
	window->setFSAA(fsaa, "");
}

void Window::move(int x, int y)
{
	window->reposition(x, y);
}

void Window::resize(unsigned int width, unsigned int height)
{
	window->resize(width, height);
}

bool Window::isClosed()
{
	return closed;
}

void Window::moved(unsigned int x, unsigned int y)
{
	this->xPosition = x;
	this->yPosition = y;
	window->windowMovedOrResized();
}

void Window::resized(unsigned int width, unsigned int height)
{
	this->width = width;
	this->height = height;
	window->windowMovedOrResized();
}

void Window::close()
{
	closed = true;
}
