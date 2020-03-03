#include <iostream>

#include "GaiaCore.h"
#include "ComponentManager.h"
#include "GaiaComponent.h"
#include "UserComponent.h"
#include "GameObject.h"
#include "SceneManager.h"

int main()
{
	GaiaCore engine;
	engine.init();

	ComponentManager::registerComponent<GaiaComponent>("GaiaComponent");
	
	SceneManager sceneManager;
	sceneManager.createScene("MainScene");

	engine.run();

	engine.close();

	return 0;
}