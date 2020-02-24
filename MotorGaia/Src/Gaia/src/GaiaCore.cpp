#include "GaiaCore.h"
#include <OgreRoot.h>

#include <iostream>

#include "RenderSystem.h"

GaiaCore::GaiaCore()
{
}

GaiaCore::~GaiaCore()
{
}

void GaiaCore::init()
{
#ifdef _DEBUG
    Ogre::Root* r = new Ogre::Root("plugins_d.cfg");
#else
    Ogre::Root* r = new Ogre::Root("plugins.cfg");
#endif

    RenderSystem::GetInstance()->printA();
}

void GaiaCore::run()
{

}

void GaiaCore::update()
{
}
