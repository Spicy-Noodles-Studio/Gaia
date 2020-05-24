#pragma once
#ifndef WINDOW_H
#define WINDOW_H
#include "WindowEventListener.h"

#include <OgreRoot.h>
#include <OgreRenderWindow.h>
#include "GaiaExports.h"

#include <SDL2/SDL.h>

typedef Ogre::Viewport Viewport;
typedef Ogre::RenderWindow RenderWindow;

#define WINDOW_CENTER_X SDL_WINDOWPOS_CENTERED
#define WINDOW_CENTER_Y SDL_WINDOWPOS_CENTERED

class Window : public WindowEventListener
{
	friend class WindowManager;
	friend class InterfaceSystem;
	friend class Cursor;
public:
	Window(Ogre::Root* root, const std::string& title);
	Window(Ogre::Root* root, const std::string& title, unsigned int width = 0, unsigned int height = 0);
	~Window();

	void setFullscreen(bool fullscreen);
	void setFSAA(unsigned int fsaa);

	void move(int x, int y);
	void resize(unsigned int width, unsigned int height);
	bool isClosed();
	void close();

	void setMinimumSize(unsigned int width, unsigned int height);
	void setWindowResizable(bool resize);

	std::pair<unsigned int, unsigned int> getResolution() const;
	std::pair<int, int> getPosition();

private:
	Viewport* addViewport(Ogre::Camera* cam);
	RenderWindow* getRenderWindow();
	SDL_Window* getSDLWindow() const;

	void removeAllViewports();

private:
	void moved(unsigned int x, unsigned int y);
	void resized(unsigned int width, unsigned int height);

private:
	Ogre::Root* root;

	SDL_Window* sdlWindow;
	RenderWindow* window;

	bool isResizable;

	int xPosition;
	int yPosition;

	unsigned int width;
	unsigned int height;

	bool closed;
};

#endif
