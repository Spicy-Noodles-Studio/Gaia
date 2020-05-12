#pragma once
#ifndef WINDOW_MANAGER_H
#define WINDOW_MANAGER_H

#include <Singleton.h>
#include <OgreRoot.h>
#include <functional>
#include <map>

#include "Component.h"
#include "DebugUtils.h"

class Camera;
class Window;

class GAIA_API WindowManager : public Singleton<WindowManager>
{
public:
	WindowManager();
	~WindowManager();

	void createWindow(Ogre::Root* root, std::string windowName);

	void initResolutions();

	void windowResize(unsigned int width, unsigned int height, int id = 0);
	void setFullscreen(bool fullscreen, int id = 0);

	bool isAllWindowsClosed();
	bool isWindowClosed(int id = 0);

	Window* getWindow(int id = 0);

	std::vector<std::pair<int, int>> getAvailableResolutionsForWindow();
	std::vector<std::string> getAvailableResolutionsStrings();

	void closeWindow(int id = 0);

	void removeAllViewportsFromWindow(int id = 0);
	Ogre::Viewport* addViewportToWindow(Camera* cam, int id = 0);

	std::pair<int, int> getActualResolution();
	bool getFullscreen();

	void setBrightness(float value);
	float getBrightness();

	void setWindowMinArea(int width, int height, int id = 0);
	void setWindowResizable(bool resize, int id = 0);

	void setActualResolutionId(int id);
	int getActualResolutionId();

	void close();

private:
	std::vector<std::pair<int, int>> resolutions;
	std::vector<Window*> windows;
	std::vector<std::string> resolutionNames;

	Ogre::Root* ogreRoot;

	std::pair<int, int> actualResolution;

	int actualResolutionId;
	bool isFullScreen;

	float brightness;
};

#endif