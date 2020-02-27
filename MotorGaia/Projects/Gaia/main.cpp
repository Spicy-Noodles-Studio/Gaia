#include <iostream>
#include <fstream>

#include "GaiaCore.h"
#include "ComponentManager.h"
#include "GaiaComponent.h"
#include "GameObject.h"
#include "DataLoader.h"
#include "GameObjectData.h"
#include "SceneData.h"

int main() 
{
	GaiaCore engine;
	engine.init();

	//EJEMPLO DE CARGAR DE ESCENA
	//Carga de componentes
	ComponentManager cM;
	cM.registerComponent<GaiaComponent>("GaiaComponent");
	//Creamos el gameObject que hay que generar (no tiene Scene, por eso el nullptr)
	GameObject* gameObject = new GameObject("Example", "Example", nullptr);
	//Creamos sus componentes y se los asignamos
	std::string componentName = "GaiaComponent";
	auto compConstructor = cM.getComponentFactory(componentName);
	GaiaComponent* gC = (GaiaComponent*)compConstructor(gameObject);
	gameObject->addComponent(componentName, gC);

	DataLoader dL;
	std::fstream i;
	i.open("Assets//Scenes//S.scene");
	if (i.is_open()) {
		json j;
		i >> j;
		SceneData c = dL.loadSceneData(j);
		i.close();
	}

	//TODO: si es posible, hacer que cada componente tenga un nombre identificador
	//dinamico, que al añadir al componentManager, se asigna y no cambia nunca mas.

	engine.run();

	delete gC;
	delete gameObject;

	return 0;
}