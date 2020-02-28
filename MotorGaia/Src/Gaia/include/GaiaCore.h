#pragma once

#ifdef GAIA_EXPORTS
#define GAIA_API __declspec(dllexport)
#else
#define GAIA_API __declspec(dllimport)
#endif

#include <OgreString.h>

class GAIA_API GaiaCore
{
public:
	GaiaCore();
	~GaiaCore();

	void setupResources();
	void init();
	void run();

private:
	Ogre::Root* mRoot;
	Ogre::String mResourcesCfg;
	Ogre::String mPluginsCfg;

	void update();

};