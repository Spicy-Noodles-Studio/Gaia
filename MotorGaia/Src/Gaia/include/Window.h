#pragma once
#ifndef WINDOW_H
#define WINDOW_H

#include <OgreRoot.h>
#include <OgreRenderWindow.h>

#include <SDL2/SDL.h>

class Window
{
public:
	Window(Ogre::Root* root, std::string windowTitle);
	~Window();

	Ogre::Viewport* addViewport(Ogre::Camera* cam);

	void setFullscreen(bool fullscreen);
	void setFSAA(unsigned int fsaa);

private:
	Ogre::Root* root;

	SDL_Window* sdlWindow;
	Ogre::RenderWindow* mWindow;

	void displayConfig(Ogre::RenderSystem* rs);
};

#endif
