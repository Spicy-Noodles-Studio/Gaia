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

	//EJEMPLO DE CARGAR DE ESCENA
	//Carga de componentes

	printf("%s\n", Component::getID<GaiaComponent>().c_str());
	printf("%s\n", Component::getID<UserComponent>().c_str());

	ComponentManager cM;
	cM.registerComponent<GaiaComponent>("GaiaComponent");
	cM.registerComponent<UserComponent>("UserComponent");

	printf("%s\n", Component::getID<GaiaComponent>().c_str());
	printf("%s\n", Component::getID<UserComponent>().c_str());
	printf("%s\n", Component::getID<GaiaComponent>().c_str());
	printf("%s\n", Component::getID<Component>().c_str());

	SceneManager sceneManager;
	ResourcesManager rManager;
	rManager.loadScene("Assets//Scenes//mainScene.scene");
	SceneData* data = ResourcesManager::getSceneData("MainScene");


	engine.run();

	return 0;
}