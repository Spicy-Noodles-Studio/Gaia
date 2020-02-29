#include <iostream>

#include "GaiaCore.h"
#include "ComponentManager.h"
#include "GaiaComponent.h"
#include "UserComponent.h"
#include "GameObject.h"
#include "SceneManager.h"
#include "ResourcesManager.h"


int main() 
{
	GaiaCore engine;
	engine.init();

	ResourcesManager rManager(".//resources.asset");
	rManager.init();

	SceneManager sceneManager;
	SceneData* data = ResourcesManager::getSceneData("Scene1");

	engine.run();

	engine.close();

	return 0;
}