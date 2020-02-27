#include "DataLoader.h"

#include <fstream>
#include <stdio.h>

#include "ResourcesManager.h"


DataLoader::DataLoader(ResourcesManager* resourcesManager) : resourcesManager(resourcesManager)
{
}

DataLoader::~DataLoader()
{
}

bool DataLoader::loadScene(const std::string& filename)
{
	std::fstream i;
	i.open(filename);
	if (!i.is_open()) {
		printf("DATA LOADER: filename %s not found\n", filename.c_str());
		return false;
	}

	json j;
	i >> j;
	resourcesManager->registerSceneData(loadSceneData(j));
	i.close();
	return true;
}

bool DataLoader::loadBlueprint(const std::string& filename)
{

	return true;
}

ComponentData* DataLoader::loadComponentData(const json& data)
{
	ComponentData* cD = new ComponentData();

	json::const_iterator name = data.find("ComponentName");
	if (name == data.end()) {
		printf("DATA LOADER: Component name not found\n");
		return cD;
	}
	cD->setName(*name);

	json::const_iterator properties = data.find("ComponentProperties");
	if (properties != data.end())
		for (auto& property : (*properties).items())
			cD->addProperty(property.key(), property.value());

	return cD;
}

GameObjectData* DataLoader::loadGameObjectData(const json& data)
{
	GameObjectData* gOD = new GameObjectData();

	//Se leen el nombre y el tag del objeto
	json::const_iterator objectName = data.find("ObjectName");
	json::const_iterator objectTag = data.find("Tag");
	if (objectName == data.end() || objectTag == data.end()) {
		printf("DATA LOADER: Object name nor tag found\n");
		//TODO: return empty GAMEOBJECTDATA instead of nullptr
		delete gOD;
		return nullptr;
	}

	gOD->setName(*objectName);
	gOD->setTag(*objectTag);

	json::const_iterator buildType = data.find("ObjectType");
	if (buildType == data.end()) {
		printf("DATA LOADER: Object type not found %s\n", gOD->getName().c_str());
		return gOD;
	}
	std::string type, aux = *buildType;
	for (char c : aux) type += std::tolower(c);

	if (type == "blueprint") {//Comprobar si existe en el json
		//Si se usa un prefab se buscan sus datos y se modifican sus propiedades 
		//gOD = resourceManager->findBlueprint(data["BlueprintPath"]);
		//Se modifican los componentes si es necesario
		json::const_iterator mod = data.find("ComponentModifications");
		if (mod != data.end())
			modifyComponents(*gOD, *mod);

		//Se modifican los hijos si es necesario
		json::const_iterator childMod = data.find("ChildrenModifications");
		if (childMod != data.end())
			for (auto& child : (*childMod).items()) {
				bool exists;
				GameObjectData& childData = gOD->getChild(child.key(), exists);
				if (exists) {//Si el hijo actual no existe
					json::const_iterator childCompMod = child.value().find("ComponentModifications");
					if (childCompMod != child.value().end())
						modifyComponents(childData, *childCompMod);

					json::const_iterator childComp = child.value().find("Components");
					if (childComp != child.value().end())
						addComponents(childData, *childComp);
				}
			}
	}
	else if (type != "gameobject") {
		printf("DATA LOADER: %s is an invalid object type\n", gOD->getName().c_str());
		//TODO: return empty GAMEOBJECTDATA instead of nullptr
		delete gOD;
		return nullptr;
	}

	//Se cargan los nuevos componentes
	json::const_iterator components = data.find("Components");
	if (components != data.end())
		addComponents(*gOD, *components);

	//Se cargan los datos de los hijos con una llamada recursiva
	json::const_iterator children = data.find("Children");
	if (children != data.end())
		for (auto& child : (*children).items()) {
			json::const_iterator name = child.value().find("ObjectName");
			if (name == child.value().end()) {
				printf("DATA LOADER: object child name %s not found\n", gOD->name.c_str());
				//TODO: return empty GAMEOBJECTDATA instead of nullptr
				return nullptr;
			}

			gOD->addChildrenData(child.value()["ObjectName"], *loadGameObjectData(child.value()));
		}

	return gOD;
}

SceneData* DataLoader::loadSceneData(const json& data)
{
	SceneData* sD = new SceneData();

	json::const_iterator name = data.find("SceneName");
	if (name == data.end()) {
		printf("DATA LOADER: SceneName not found\n");
		return sD;
	}
	sD->setName(*name);

	json::const_iterator objects = data.find("SceneObjects");
	if (objects != data.end())
		for (auto& gameObject : (*objects).items()) {
			sD->setGameObjectData(*loadGameObjectData(gameObject.value()));
		}

	return sD;
}

void DataLoader::addComponents(GameObjectData& gOD, const json& data)
{
	for (auto& component : data.items()) {
		json::const_iterator name = component.value().find("ComponentName");
		if (name == component.value().end()) {
			printf("DATA LOADER: ComponentName not found\n");
			return;
		}

		gOD.addComponentData(*name, *loadComponentData(component.value()));
	}
}

void DataLoader::modifyComponents(GameObjectData& gOD, const json& data)
{
	for (auto& component : data.items()) {
		json::const_iterator name = component.value().find("ComponentName");
		if (name == component.value().end()) {
			printf("DATA LOADER: ComponentName not found for modification\n");
			return;
		}
		json::const_iterator properties = component.value().find("ComponentProperties");
		if (properties != component.value().end()) {
			for (auto& property : (*properties).items()) {
				if (!gOD.modifyComponentData(*name, property.key(), property.value()))//Si no se ha podido modificar el componente pasamos al siguiente
					break;
			}
		}
	}
}

