#include "Window.h"

#include <OgreRenderSystem.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_video.h>
#include <SDL2/SDL_syswm.h>
#include <OgreViewport.h>

#include <iostream>

Window::Window(Ogre::Root* root, const std::string& title) : root(root), closed(false), isResizable(true)
{
	Ogre::RenderSystem* rs = root->getRenderSystem();
	Ogre::ConfigOptionMap ropts = rs->getConfigOptions();
	std::istringstream mode(ropts["Video Mode"].currentValue);
	Ogre::String token;
	int w, h;
	mode >> w; // width
	mode >> token; // 'x' separator
	mode >> h; // height

	this->width = w;
	this->height = h;

	Uint32 flags = SDL_WINDOW_RESIZABLE;
	sdlWindow = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, this->width, this->height, flags);

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

	window = root->createRenderWindow(title.c_str(), this->width, this->height, false, &params);

	SDL_GetWindowPosition(sdlWindow, &xPosition, &yPosition);

	// Ensure Ogre does not change window size
	resize(this->width, this->height);

	// Callbacks definition
	onWindowSizeChanged([this](unsigned int width, unsigned int height) { resized(width, height); });
	onWindowResized([this](unsigned int width, unsigned int height) { resized(width, height); });
	onWindowMoved([this](int x, int y) { moved(x, y); });
	onWindowClose([this]() { close(); });
}

Window::Window(Ogre::Root* root, const std::string& title, unsigned int width, unsigned int height) : root(root), closed(false), isResizable(true)
{
	Ogre::RenderSystem* rs = root->getRenderSystem();
	Ogre::ConfigOptionMap ropts = rs->getConfigOptions();

	if (width == 0 || height == 0) {
		std::istringstream mode(ropts["Video Mode"].currentValue);
		Ogre::String token;
		int w, h;
		mode >> w; // width
		mode >> token; // 'x' separator
		mode >> h; // height
		this->width = w;
		this->height = h;
	}
	else {
		this->width = width;
		this->height = height;
	}

	Uint32 flags = SDL_WINDOW_RESIZABLE;
	sdlWindow = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, this->width, this->height, flags);

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

	window = root->createRenderWindow(title.c_str(), this->width, this->height, false, &params);

	SDL_GetWindowPosition(sdlWindow, &xPosition, &yPosition);

	// Ensure Ogre does not change window size
	resize(this->width, this->height);

	// Callbacks definition
	onWindowSizeChanged([this](unsigned int width, unsigned int height) { resized(width, height); });
	onWindowResized([this](unsigned int width, unsigned int height) { resized(width, height); });
	onWindowMoved([this](int x, int y) { moved(x, y); });
	onWindowClose([this]() { close(); });
}

Window::~Window()
{
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

SDL_Window* Window::getSDLWindow() const
{
	return sdlWindow;
}

void Window::removeAllViewports()
{
	window->removeAllViewports();
}

void Window::setFullscreen(bool fullscreen)
{
	SDL_SetWindowResizable(sdlWindow, SDL_bool(true));
	if (fullscreen)
	{
		SDL_SetWindowFullscreen(sdlWindow, SDL_WINDOW_FULLSCREEN);
	}
	else
	{
		SDL_SetWindowFullscreen(sdlWindow, !SDL_WINDOW_FULLSCREEN);
	}
	SDL_SetWindowResizable(sdlWindow, SDL_bool(isResizable));
}

void Window::setFSAA(unsigned int fsaa)
{
	window->setFSAA(fsaa, "");
}

void Window::move(int x, int y)
{
	SDL_SetWindowPosition(sdlWindow, x, y);
}

void Window::resize(unsigned int width, unsigned int height)
{
	SDL_SetWindowResizable(sdlWindow, SDL_bool(true));
	SDL_SetWindowSize(sdlWindow, width, height);
	SDL_SetWindowResizable(sdlWindow, SDL_bool(isResizable));
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

void Window::setMinimumSize(unsigned int width, unsigned int height)
{
	SDL_SetWindowMinimumSize(sdlWindow, width, height);
}

void Window::setWindowResizable(bool resize)
{
	SDL_bool resizable = (SDL_bool)resize;
	SDL_SetWindowResizable(sdlWindow, resizable);
	isResizable = resize;
}

std::pair<unsigned int, unsigned int> Window::getResolution() const
{
	return { width, height };
}

std::pair<int, int> Window::getPosition()
{
	SDL_GetWindowPosition(sdlWindow, &xPosition, &yPosition);
	return { xPosition, yPosition };
}
