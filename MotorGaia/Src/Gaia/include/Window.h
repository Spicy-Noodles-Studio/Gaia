#pragma once
#ifndef WINDOW_H
#define WINDOW_H
#include "WindowEventListener.h"

#include <OgreRoot.h>
#include <OgreRenderWindow.h>

#include <SDL2/SDL.h>

typedef Ogre::Viewport Viewport;
typedef Ogre::RenderWindow RenderWindow;

class Window : public WindowEventListener
{
public:
	Window(Ogre::Root* root, std::string windowTitle);
	~Window();

	Viewport* addViewport(Ogre::Camera* cam);
	RenderWindow* getRenderWindow();

	void removeAllViewports();

	void setFullscreen(bool fullscreen);
	void setFSAA(unsigned int fsaa);

	void move(int x, int y);
	void resize(unsigned int width, unsigned int height);
	bool isClosed();

private:
	void moved(unsigned int x, unsigned int y);
	void resized(unsigned int width, unsigned int height);
	void close();

private:
	Ogre::Root* root;

	SDL_Window* sdlWindow;
	RenderWindow* window;

	int xPosition;
	int yPosition;

	unsigned int width;
	unsigned int height;

	bool closed;
};

#endif
