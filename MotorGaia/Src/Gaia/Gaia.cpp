// Gaia.cpp : Este archivo contiene la función "main". La ejecución del programa comienza y termina ahí.
//
#include <OgreRoot.h>
#include <iostream>
#include <fmod.h>
#include <fmod.hpp>
#include <b3Random.h>


#ifdef _DEBUG
int main()
#else
#include <Windows.h>
int WINAPI
WinMain(HINSTANCE hinstance, HINSTANCE prevInstance, LPSTR lpCmdLine, int nCmdShow)
#endif
{



    std::cout << "Hello World!\n";
#ifdef _DEBUG
    Ogre::Root* r = new Ogre::Root("plugins_d.cfg");
#else
    Ogre::Root* r = new Ogre::Root("plugins.cfg");
#endif
    b3rand();
    FMOD::System* xsystem;
}

// Ejecutar programa: Ctrl + F5 o menú Depurar > Iniciar sin depurar
// Depurar programa: F5 o menú Depurar > Iniciar depuración

// Sugerencias para primeros pasos: 1. Use la ventana del Explorador de soluciones para agregar y administrar archivos
//   2. Use la ventana de Team Explorer para conectar con el control de código fuente
//   3. Use la ventana de salida para ver la salida de compilación y otros mensajes
//   4. Use la ventana Lista de errores para ver los errores
//   5. Vaya a Proyecto > Agregar nuevo elemento para crear nuevos archivos de código, o a Proyecto > Agregar elemento existente para agregar archivos de código existentes al proyecto
//   6. En el futuro, para volver a abrir este proyecto, vaya a Archivo > Abrir > Proyecto y seleccione el archivo .sln
