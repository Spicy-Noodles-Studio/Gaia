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

    if (!(r->restoreConfig() || r->showConfigDialog(nullptr)))
        return;

    r->initialise(false);

}

void GaiaCore::run()
{

}

void GaiaCore::close()
{

}

void GaiaCore::update()
{
}
