#include "WindowManager.h"
#include"Window.h"
#include <OgreRoot.h>
#include <OgreConfig.h>
#include <OgreConfigFile.h>
#include <OgreConfigDialog.h>
#include <OgreConfigOptionMap.h>
#include <SDL2/SDL_syswm.h>
#include "Camera.h"
WindowManager::WindowManager()
{
}

WindowManager::~WindowManager()
{
}



void WindowManager::createWindow(Ogre::Root* root, std::string windowName)
{
	_root = root;
	windows.push_back(new Window(root, "Test window - 2020 (c) Gaia "));
}

void WindowManager::initResolutions()
{
	isFullScreen = false;
	brightness = 1;
	actualResolutionId = 0;

	Ogre::ConfigOptionMap mapa = _root->getRenderSystem()->getConfigOptions();
	Ogre::ConfigOptionMap::iterator configIt = mapa.begin();
	while (configIt != mapa.end())
	{
		if (configIt->first == "Video Mode")
		{
			resolString = configIt->second.possibleValues;
		}
		configIt++;
	}
	std::vector<std::pair<int, int>> resolutions;
	std::pair<int, int> pareja;
	for (std::string res : resolString)
	{
		int resX = 0;
		int resY = 0;
		int it = 0;
		bool notANumber = false;
		while (it < res.length() && !notANumber)
		{
			if (res.at(it) != 'x' && res.at(it) != 'X' && res.at(it) != ' ') {
				resX += (int)res.at(it)-'0';
				resX = resX * 10;
			
			}
			else if(res.at(it) == 'x'|| res.at(it) == 'X')
				notANumber = true;
			it++;
		}
		notANumber = false;
		while (it < res.length())
		{
			if (res.at(it) != 'x' && res.at(it) != 'X' && res.at(it) != ' ') {
				resY += (int)res.at(it)-'0';
				resY = resY * 10;
			}
			it++;
		}
		pareja.first = resX/10;
		pareja.second = resY/10;
		resolutions.push_back(pareja);
	}
	_resolutions = resolutions;
}

void WindowManager::windowResize(unsigned int width, unsigned int height,int id)
{
	windows.at(id)->resize(width, height);
	actualResolution.first = width;
	actualResolution.second = height;
}

void WindowManager::setFullscreen(bool fullscreen,int id)
{
	windows.at(id)->setFullscreen(fullscreen);
	isFullScreen = fullscreen;
}

bool WindowManager::isAllWindowsClosed()
{
	bool closed = false;
	int i = 0;
	while (!closed && windows.size() > i)
	{
		closed = windows.at(i);
		i++;
	}
	return closed;
}

bool WindowManager::isWindowClosed(int id)
{
	return windows.at(id)->isClosed();
}

Window* WindowManager::getWindow(int id)
{
	return windows.at(id);
}

std::vector<std::pair<int, int>> WindowManager::getAvailableResolutionsForWindow()
{
	return _resolutions;
}

std::vector<std::string> WindowManager::getAvailableResolutionsStrings()
{
	return resolString;
}

void WindowManager::closeWindow(int id)
{
	windows.at(id)->close();
}



void WindowManager::removeAllViewportsFromWindow(int id)
{
	windows.at(id)->removeAllViewports();
}

Ogre::Viewport* WindowManager::addViewportToWindow(Camera* cam, int id)
{
	return windows.at(id)->addViewport(cam->getCamera());
}

std::pair<int, int> WindowManager::getActualResolution()
{
	return actualResolution;
}

bool WindowManager::getFullscreen()
{
	return isFullScreen;
}

void WindowManager::setBrightness(float value)
{
	brightness = value;
}

float WindowManager::getBrightness()
{
	return brightness;
}

void WindowManager::setActualResolutionId(int id)
{
	actualResolutionId = id;
}

int WindowManager::getActualResolutionId()
{
	return actualResolutionId;
}

void WindowManager::close()
{
	for(Window* var: windows)
	{
		if (var != nullptr)
		{
			delete var;
			var = nullptr;
		}
	}
	windows.clear();

	destroy();
}

