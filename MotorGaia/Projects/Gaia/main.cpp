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

	BlueprintData nudoBP;
	BlueprintData sinbadBP;
	nudoBP.locate("./Assets/Blueprints/nudo.blueprint");
	sinbadBP.locate("./Assets/Blueprints/Sinbad.blueprint");
	nudoBP.loadAsync();
	sinbadBP.loadAsync();
	ResourcesManager::blueprintData[nudoBP.id] = &nudoBP;
	ResourcesManager::blueprintData[sinbadBP.id] = &sinbadBP;

	SceneData sceneData;
	sceneData.locate("./Assets/Scenes/mainScene.scene");
	sceneData.load();
	
	
	GaiaCore engine;
	engine.init();

	engine.run();

	engine.close();


	return 0;
}