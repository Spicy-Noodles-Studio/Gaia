#include <iostream>

#include "GaiaCore.h"
#include "ComponentManager.h"
#include "GaiaComponent.h"
#include "UserComponent.h"
#include "SceneManager.h"

#include <stdlib.h>
#include <crtdbg.h>


int main()
{
	/*_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#define _CRTDBG_MAP_ALLOC*/

	GaiaCore engine;
	engine.init();

	ComponentManager::registerComponent<GaiaComponent>("GaiaComponent");

	SceneManager sceneManager;
	sceneManager.createScene("MainScene");

	engine.run();

	engine.close();

	return 0;
}