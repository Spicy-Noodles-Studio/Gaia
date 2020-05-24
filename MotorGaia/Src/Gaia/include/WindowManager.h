#pragma once
#ifndef WINDOW_MANAGER_H
#define WINDOW_MANAGER_H

#include <Singleton.h>
#include "DebugUtils.h"
#include <vector>

namespace Ogre {
	class Root;
	class Viewport;
}

class Camera;
class Window;

class GAIA_API WindowManager : public Singleton<WindowManager>
{
	friend class GaiaCore;
	friend class SceneManager;
public:
	WindowManager();
	~WindowManager();

	Window* createWindow(const std::string& title, unsigned int width = 0, unsigned int height = 0);

	void windowResize(unsigned int width, unsigned int height, int id = 0);
	void setFullscreen(bool fullscreen, int id = 0);

	std::pair<unsigned int, unsigned int> getActualResolucion() const;
	bool getFullscreen() const;

	void setBrightness(float value);
	float getBrightness() const;

	void closeWindow(int id = 0);

	bool allClosed() const;
	bool isClosed(int id = 0) const;

	Window* getWindow(int id = 0);
	const std::vector<std::pair<unsigned int, unsigned int>>& getAvailableResolutionsForWindow() const;
	const std::vector<std::string>& getAvailableResolutionsStrings() const;

	void setMinimumSize(unsigned int width, unsigned int height, int id = 0);
	void setWindowResizable(bool resize, int id = 0);

	void setActualResolutionId(int id);
	int getActualResolutionId() const;

private:
	void init(Ogre::Root* root);
	void close();

	void initResolutions();

	void removeAllViewportsFromWindow(int id = 0);
	Ogre::Viewport* addViewportToWindow(Camera* camera, int id = 0);

private:
	Ogre::Root* root;

	std::vector<Window*> windows;
	std::vector<std::pair<unsigned int, unsigned int>> resolutions;
	std::vector<std::string> resolutionNames;

	bool isFullScreen;
	float brightness;
};

#endif
