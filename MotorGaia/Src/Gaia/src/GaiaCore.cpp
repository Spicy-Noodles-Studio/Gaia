#include "GaiaCore.h"
#include <OgreRoot.h>

#include <iostream>

GaiaCore::GaiaCore()
{
}

void GaiaCore::init()
{
#ifdef _DEBUG
    Ogre::Root* r = new Ogre::Root("plugins_d.cfg");
#else
    Ogre::Root* r = new Ogre::Root("plugins.cfg");
#endif
}

void GaiaCore::update()
{
}
