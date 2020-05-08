#include "WindowManager.h"
#include "Window.h"
#include "Camera.h"

#include <OgreRoot.h>
#include <OgreConfig.h>
#include <OgreConfigFile.h>
#include <OgreConfigDialog.h>
#include <OgreConfigOptionMap.h>
#include <SDL2/SDL_syswm.h>

WindowManager::WindowManager()
{

}

WindowManager::~WindowManager()
{

}

void WindowManager::createWindow(Ogre::Root* root, std::string windowName)
{
	ogreRoot = root;
	windows.push_back(new Window(root, windowName));
}

void WindowManager::initResolutions()
{
	isFullScreen = false;
	brightness = 1;
	actualResolutionId = 0;

	Ogre::ConfigOptionMap map = ogreRoot->getRenderSystem()->getConfigOptions();
	Ogre::ConfigOptionMap::iterator configIt = map.begin();

	while (configIt != map.end())
	{
		if (configIt->first == "Video Mode")
			resolutionNames = configIt->second.possibleValues;

		configIt++;
	}

	std::vector<std::pair<int, int>> resolutionsVector;
	std::pair<int, int> resolutionPair;

	for (std::string res : resolutionNames)
	{
		int resX = 0;
		int resY = 0;
		int it = 0;
		bool notANumber = false;

		while (it < res.length() && !notANumber)
		{
			if (res.at(it) != 'x' && res.at(it) != 'X' && res.at(it) != ' ')
			{
				resX += (int)res.at(it) - '0';
				resX = resX * 10;
			}
			else if (res.at(it) == 'x' || res.at(it) == 'X')
				notANumber = true;

			it++;
		}

		notANumber = false;
		while (it < res.length())
		{
			if (res.at(it) != 'x' && res.at(it) != 'X' && res.at(it) != ' ')
			{
				resY += (int)res.at(it) - '0';
				resY = resY * 10;
			}
			it++;
		}

		resolutionPair.first = resX / 10;
		resolutionPair.second = resY / 10;
		resolutionsVector.push_back(resolutionPair);
	}
	resolutions = resolutionsVector;
}

void WindowManager::windowResize(unsigned int width, unsigned int height, int id)
{
	windows.at(id)->resize(width, height);
	actualResolution.first = width;
	actualResolution.second = height;
}

void WindowManager::setFullscreen(bool fullscreen, int id)
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
	return resolutions;
}

std::vector<std::string> WindowManager::getAvailableResolutionsStrings()
{
	return resolutionNames;
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

void WindowManager::setWindowMinArea(int width, int height, int id)
{
	windows.at(id)->setWindowMinArea(width, height);
}

void WindowManager::setWindowResizable(bool resize, int id)
{
	windows.at(id)->setWindowResizable(resize);
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
	for (Window* var : windows)
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