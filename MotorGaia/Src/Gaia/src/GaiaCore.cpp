#include "GaiaCore.h"
#include <OgreRoot.h>

#include <iostream>

GaiaCore::GaiaCore()
{
}

GaiaCore::~GaiaCore()
{
    delete win;
}

void GaiaCore::init()
{
#ifdef _DEBUG
    Ogre::Root* r = new Ogre::Root("plugins_d.cfg", "window_d.cfg");
#else
    Ogre::Root* r = new Ogre::Root("plugins.cfg", "window.cfg");
#endif

    if (!(r->restoreConfig() || r->showConfigDialog(nullptr)))
        return;

    win = new Window(r, "Ventana de Prueba");

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
