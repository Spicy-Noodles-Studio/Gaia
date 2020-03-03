#include "DataLoader.h"

#include <fstream>
#include <stdio.h>

#include "ResourcesManager.h"


DataLoader::DataLoader()
{
}

DataLoader::~DataLoader()
{
}

SceneData* DataLoader::loadScene(const std::string& filename, bool& loaded)
{
	std::fstream i;
	i.open(filename);
	if (!i.is_open()) {
		printf("DATA LOADER: filename %s not found\n", filename.c_str());
		loaded = false;
		return SceneData::empty();
	}

	json j;
	i >> j;
	i.close();
	return loadSceneData(j, loaded);
}

GameObjectData* DataLoader::loadBlueprint(const std::string& filename, bool& loaded)
{
	std::fstream i;
	i.open(filename);
	if (!i.is_open()) {
		printf("DATA LOADER: filename %s not found\n", filename.c_str());
		loaded = false;
		return GameObjectData::empty();
	}

	json j;
	i >> j;
	i.close();
	return loadGameObjectData(j, loaded);
}

ComponentData* DataLoader::loadComponentData(const json& data, bool& loaded)
{
	ComponentData* cD = new ComponentData();

	json::const_iterator name = data.find("ComponentName");
	if (name == data.end()) {
		printf("DATA LOADER: Component name not found\n");
		loaded = false;
		return cD;
	}
	cD->setName(*name);

	json::const_iterator properties = data.find("ComponentProperties");
	if (properties != data.end())
		for (auto& property : (*properties).items())
			loaded = cD->addProperty(property.key(), property.value()) && loaded;//Cuidado evaluacion perezosa

	return cD;
}

GameObjectData* DataLoader::loadGameObjectData(const json& data, bool& loaded)
{
	GameObjectData* gOD = new GameObjectData();

	//Se leen el nombre y el tag del objeto
	json::const_iterator objectName = data.find("ObjectName");
	json::const_iterator objectTag = data.find("Tag");
	if (objectName == data.end() || objectTag == data.end()) {
		printf("DATA LOADER: Object name nor tag found\n");
		loaded = false;
		delete gOD;
		return GameObjectData::empty();
	}

	gOD->setName(*objectName);
	gOD->setTag(*objectTag);

	json::const_iterator buildType = data.find("ObjectType");
	if (buildType == data.end()) {
		printf("DATA LOADER: Object type not found %s\n", gOD->getName().c_str());
		loaded = false;
		return gOD;
	}
	std::string type, aux = *buildType;
	for (char c : aux) type += std::tolower(c);

	if (type == "blueprint") {//Comprobar si existe en el json
		//Si se usa un prefab se buscan sus datos y se modifican sus propiedades 
		delete gOD;
		const GameObjectData* blueprint = ResourcesManager::getBlueprint(data["BlueprintPath"]);
		if (blueprint == nullptr)//Si el blueprint no ha sido cargado
			return GameObjectData::empty();
		gOD = new GameObjectData(*blueprint); //Copia del blueprint
		gOD->setName(*objectName);
		gOD->setTag(*objectTag);

		//Se modifican los componentes si es necesario
		json::const_iterator mod = data.find("ComponentModifications");
		if (mod != data.end())
			loaded = modifyComponents(*gOD, *mod);

		//Se modifican los hijos si es necesario
		json::const_iterator childMod = data.find("ChildrenModifications");
		if (childMod != data.end())
			for (auto& child : (*childMod).items()) {
				bool exists;
				GameObjectData* childData = gOD->getChild(child.key(), exists);
				if (exists) {//Si el hijo actual existe
					json::const_iterator childCompMod = child.value().find("ComponentModifications");
					if (childCompMod != child.value().end())
						loaded = modifyComponents(*childData, *childCompMod) && loaded;

					json::const_iterator childComp = child.value().find("Components");
					if (childComp != child.value().end())
						loaded = addComponents(*childData, *childComp) && loaded;
				}
			}
	}
	else if (type != "gameobject") {
		printf("DATA LOADER: %s is an invalid object type\n", gOD->getName().c_str());
		loaded = false;
		delete gOD;
		return GameObjectData::empty();
	}

	//Se cargan los nuevos componentes
	json::const_iterator components = data.find("Components");
	if (components != data.end())
		loaded = addComponents(*gOD, *components) && loaded;

	//Se cargan los datos de los hijos con una llamada recursiva
	json::const_iterator children = data.find("Children");
	if (children != data.end())
		for (auto& child : (*children).items()) {
			json::const_iterator name = child.value().find("ObjectName");
			if (name == child.value().end()) {
				printf("DATA LOADER: object child name %s not found\n", gOD->name.c_str());
				loaded = false;
				delete gOD;
				return GameObjectData::empty();
			}
			bool aux = true;
			gOD->addChildrenData(*name, loadGameObjectData(child.value(), aux));
			loaded = loaded && aux;
		}

	return gOD;
}

SceneData* DataLoader::loadSceneData(const json& data, bool& loaded)
{
	SceneData* sD = new SceneData();

	json::const_iterator name = data.find("SceneName");
	if (name == data.end()) {
		printf("DATA LOADER: SceneName not found\n");
		loaded = false;
		return SceneData::empty();
	}
	sD->setName(*name);

	json::const_iterator objects = data.find("SceneObjects");
	if (objects != data.end()) {
		bool aux = true;
		for (auto& gameObject : (*objects).items()) {
			sD->setGameObjectData(loadGameObjectData(gameObject.value(), aux));
			loaded = aux && loaded;
		}
	}

	return sD;
}

bool DataLoader::addComponents(GameObjectData& gOD, const json& data)
{
	bool allAdded = true;
	for (auto& component : data.items()) {
		json::const_iterator name = component.value().find("ComponentName");
		if (name == component.value().end()) {
			printf("DATA LOADER: ComponentName not found\n");
			return false;
		}
		bool aux = true;
		gOD.addComponentData(*name, loadComponentData(component.value(), aux));
		allAdded = allAdded && aux;
	}
	return allAdded;
}

bool DataLoader::modifyComponents(GameObjectData& gOD, const json& data)
{
	bool allMod = true;
	for (auto& component : data.items()) {
		json::const_iterator name = component.value().find("ComponentName");
		if (name == component.value().end()) {
			printf("DATA LOADER: ComponentName not found for modification\n");
			allMod = false;
			continue; // Si no se ha podido modificar el componente pasamos al siguiente
		}
		json::const_iterator properties = component.value().find("ComponentProperties");
		if (properties != component.value().end()) {
			for (auto& property : (*properties).items()) {
				if (!gOD.modifyComponentData(*name, property.key(), property.value())) {//Si no se ha podido modificar una propiedad del componente pasamos a la siguiente
					allMod = false;
					break;
				}
			}
		}
	}
	return allMod;
}

