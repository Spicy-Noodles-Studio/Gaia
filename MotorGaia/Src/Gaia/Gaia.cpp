#include <OgreRoot.h>
#include <iostream>


#ifdef _DEBUG
int main()
#else
#include <Windows.h>
int WINAPI
WinMain(HINSTANCE hinstance, HINSTANCE prevInstance, LPSTR lpCmdLine, int nCmdShow)
#endif
{

#ifdef _DEBUG
    Ogre::Root* r = new Ogre::Root("plugins_d.cfg");
#else
    Ogre::Root* r = new Ogre::Root("plugins.cfg");
#endif
    


}

