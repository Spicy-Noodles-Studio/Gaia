#include "Window.h"

#include <OgreRenderSystem.h>
#include <Windows.h>

Window::Window(Ogre::Root* mRoot)
{
	// Do not add this to your project
	Ogre::RenderSystem* rs = mRoot->getRenderSystemByName("GL Rendering Subsystem");

	mRoot->setRenderSystem(rs);
	rs->setConfigOption("Full Screen", "No");
	rs->setConfigOption("Video Mode", "800 x 600 @ 32-bit colour");

	mWindow = mRoot->initialise(true, "TutorialApplication Render Window");

	// Do not add this to your project
	mRoot->initialise(false);

	HWND hWnd = 0;

	// Retrieve the HWND for the window we want to render in.
	// This step depends entirely on the windowing system you are using.

	Ogre::NameValuePairList misc;
	misc["externalWindowHandle"] = Ogre::StringConverter::toString((int)hWnd);

	Ogre::RenderWindow* win = mRoot->createRenderWindow("Main RenderWindow", 800, 600, false, &misc);
}

Window::~Window()
{

}

