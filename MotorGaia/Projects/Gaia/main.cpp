#ifdef _DEBUG
#include <cstdlib>
#include <crtdbg.h>
#define _CRTDBG_MAP_ALLOC
#endif

#include <iostream>
#include "GaiaCore.h"
#include "BlueprintData.h"


#ifdef _DEBUG
int main()
#else
#include <Windows.h>
int WINAPI
WinMain(HINSTANCE hinstance, HINSTANCE prevInstance, LPSTR lpCmdLine, int nCmdShow)
#endif
{
#ifdef _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_CrtSetBreakAlloc(94345);
#endif

	BlueprintData data;
	data.locate("./Assets/Blueprints/nudo.blueprint");
	data.loadAsync();
	
	
	GaiaCore engine;
	engine.init();

	engine.run();

	engine.close();


	return 0;
}