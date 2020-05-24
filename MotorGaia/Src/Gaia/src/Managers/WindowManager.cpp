#include "WindowManager.h"
#include <algorithm>
#include "Window.h"
#include "Camera.h"

#include <OgreRoot.h>
#include <OgreConfig.h>
#include <OgreConfigFile.h>
#include <OgreConfigDialog.h>
#include <OgreConfigOptionMap.h>
#include <SDL2/SDL_syswm.h>

WindowManager::WindowManager() : brightness(1.0f), isFullScreen(false), root(nullptr)
{

}

WindowManager::~WindowManager()
{

}

Window* WindowManager::createWindow(const std::string& title, unsigned int width, unsigned int height)
{
	checkNullAndBreak(root, nullptr);

	if (!resolutions.size()) {
		LOG_ERROR("WINDOW MANAGER", "Resolutions vector not initialized");
		return nullptr;
	}
	
	if (width == 0 || height == 0) {
		auto maxResolution = *(resolutions.end() - 1);
		width = maxResolution.first;
		height = maxResolution.second;
	}

	Window* window = new Window(root, title, width, height);
	window->setMinimumSize(resolutions[0].first, resolutions[0].second);

	windows.push_back(window);
	return window;
}

void WindowManager::windowResize(unsigned int width, unsigned int height, int id)
{
	if (id >= windows.size()) {
		LOG_ERROR("WINDOW MANAGER", "Trying to access to a non existing window");
		return;
	}

	windows[id]->resize(width, height);
}

void WindowManager::setFullscreen(bool fullscreen, int id)
{
	if (id >= windows.size()) {
		LOG_ERROR("WINDOW MANAGER", "Trying to access to a non existing window");
		return;
	}

	windows[id]->setFullscreen(fullscreen);
	isFullScreen = fullscreen;
}

std::pair<unsigned int, unsigned int> WindowManager::getActualResolucion() const
{
	if (!windows.size()) {
		LOG_ERROR("WINDOW MANAGER", "Trying to access to a non existing window");
		return { 0, 0 };
	}

	return windows[0]->getResolution();
}

bool WindowManager::getFullscreen() const
{
	return isFullScreen;
}

void WindowManager::setBrightness(float value)
{
	brightness = value;
}

float WindowManager::getBrightness() const
{
	return brightness;
}

void WindowManager::closeWindow(int id)
{
	if (id >= windows.size()) {
		LOG_ERROR("WINDOW MANAGER", "Trying to access to a non existing window");
		return;
	}

	windows[id]->close();
}

bool WindowManager::allClosed() const
{
	for (Window* window : windows) {
		checkNullAndBreak(window, false);
		if (!window->isClosed())
			return false;
	}

	return true;
}

bool WindowManager::isClosed(int id) const
{
	if (id >= windows.size()) {
		LOG_ERROR("WINDOW MANAGER", "Trying to access to a non existing window");
		return false;
	}

	return windows[id]->isClosed();
}

Window* WindowManager::getWindow(int id)
{
	if (id >= windows.size()) {
		LOG_ERROR("WINDOW MANAGER", "Trying to access to a non existing window");
		return nullptr;
	}

	return windows[id];
}

const std::vector<std::pair<unsigned int, unsigned int>>& WindowManager::getAvailableResolutionsForWindow() const
{
	return resolutions;
}

const std::vector<std::string>& WindowManager::getAvailableResolutionsStrings() const
{
	return resolutionNames;
}

void WindowManager::setMinimumSize(unsigned int width, unsigned int height, int id)
{
	if (id >= windows.size()) {
		LOG_ERROR("WINDOW MANAGER", "Trying to access to a non existing window");
		return;
	}

	windows[id]->setMinimumSize(width, height);
}

void WindowManager::setWindowResizable(bool resize, int id)
{
	if (id >= windows.size()) {
		LOG_ERROR("WINDOW MANAGER", "Trying to access to a non existing window");
		return;
	}

	windows[id]->setWindowResizable(resize);
}

void WindowManager::init(Ogre::Root* root)
{
	checkNullAndBreak(root);
	this->root = root;

	Ogre::RenderSystem* rs = root->getRenderSystem();
	root->setRenderSystem(rs);

	root->initialise(false);

	SDL_SetMainReady();
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		OGRE_EXCEPT(Ogre::Exception::ERR_INTERNAL_ERROR, "Cannot initialize SDL2!",
			"BaseApplication::setup");
	}

	initResolutions();
}

void WindowManager::close()
{
	for (Window* window : windows) {
		if (window != nullptr) {
			delete window;
			window = nullptr;
		}
	}
	windows.clear();

	destroy();
}

void WindowManager::initResolutions()
{
	isFullScreen = false;
	brightness = 1.0f;

	Ogre::ConfigOptionMap map = root->getRenderSystem()->getConfigOptions();
	Ogre::ConfigOptionMap::iterator configIt = map.find("Video Mode");

	if (configIt != map.end())
		resolutionNames = configIt->second.possibleValues;

	std::vector<std::pair<unsigned int, unsigned int>> resolutionsVector;
	std::pair<unsigned int, unsigned int> resolutionPair;

	for (std::string resolution : resolutionNames)
	{
		std::stringstream ss(resolution);
		unsigned int width = 0;
		unsigned int height = 0;
		char token = ' ';

		if (!(ss >> width >> token >> height)) {
			LOG_ERROR("WINDOW MANAGER", "Error ocurred while initializing resolutions");
			return;
		}
		resolutions.push_back({ width, height });
	}
}

void WindowManager::removeAllViewportsFromWindow(int id)
{
	if (id >= windows.size()) {
		LOG_ERROR("WINDOW MANAGER", "Trying to access to a non existing window");
		return;
	}

	windows[id]->removeAllViewports();
}

Ogre::Viewport* WindowManager::addViewportToWindow(Camera* camera, int id)
{
	if (id >= windows.size()) {
		LOG_ERROR("WINDOW MANAGER", "Trying to access to a non existing window");
		return nullptr;
	}
	checkNullAndBreak(camera, nullptr);

	return windows[id]->addViewport(camera->getCamera());
}

void WindowManager::setActualResolutionId(int id)
{
	if (id >= resolutions.size()) {
		LOG_ERROR("WINDOW MANAGER", "Trying to access to a non existing resolution");
		return;
	}

	if (!windows.size()) {
		LOG_ERROR("WINDOW MANAGER", "Trying to access to a non existing window");
		return;
	}
	auto resolution = resolutions[id];
	windows[0]->resize(resolution.first, resolution.second);
}

int WindowManager::getActualResolutionId() const
{
	if (!windows.size()) {
		LOG_ERROR("WINDOW MANAGER", "Trying to access to a non existing window");
		return -1;
	}
	std::pair<unsigned int, unsigned int> resolution = windows[0]->getResolution();
	
	auto it = std::find(resolutions.begin(), resolutions.end(), resolution);
	if (it == resolutions.end()) {
		LOG_ERROR("WINDOW MANAGER", "Resolution \"%i x %i\" not detected", resolution.first, resolution.second);
		return -1;
	}

	return it - resolutions.begin();
}
