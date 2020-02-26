#include "DataLoader.h"

#include <stdio.h>

#include "ComponentData.h"
#include "GameObjectData.h"
#include "SceneData.h"

using const_iterator = nlohmann::detail::iter_impl<const nlohmann::json>;

DataLoader::DataLoader()
{
}

DataLoader::~DataLoader()
{
}

ComponentData DataLoader::loadComponentData(const json& data)
{
	ComponentData cD = ComponentData();

	const_iterator name = data.find("ComponentName");
	if (name == data.end()) {
		printf("ERROR: No se encuentra el nombre del componente");
		return cD;
	}
	cD.setName(*name);

	const_iterator properties = data.find("ComponentProperties");
	if (properties != data.end())
		for (auto& property : (*properties).items())
			cD.addProperty(property.key(), property.value());

	return cD;
}

GameObjectData DataLoader::loadGameObjectData(const json& data)
{
	GameObjectData gOD = GameObjectData();

	//Se leen el nombre y el tag del objeto
	const_iterator objectName = data.find("ObjectName");
	const_iterator objectTag = data.find("Tag");
	if (objectName == data.end() || objectTag == data.end()) {
		printf("ERROR: No se ha podido encontrar el nombre o el tag del objeto.\n");
		return GameObjectData();
	}

	gOD.setName(*objectName);
	gOD.setTag(*objectTag);

	const_iterator buildType = data.find("ObjectType");
	if (buildType == data.end()) {
		printf("ERROR: No se encuentra el tipo del objeto %s.\n", gOD.getName().c_str());
		return gOD;
	}
	std::string type, aux = *buildType;
	for (char c : aux) type += std::tolower(c);

	if (type == "blueprint") {//Comprobar si existe en el json
		//Si se usa un prefab se buscan sus datos y se modifican sus propiedades 
		//gOD = resourceManager->findBlueprint(data["BlueprintPath"]);
		//Se modifican los componentes si es necesario
		const_iterator mod = data.find("ComponentModifications");
		if (mod != data.end())
			modifyComponents(gOD, *mod);

		//Se modifican los hijos si es necesario
		const_iterator childMod = data.find("ChildrenModifications");
		if (childMod != data.end())
			for (auto& child : (*childMod).items()) {
				bool exists;
				GameObjectData& childData = gOD.getChild(child.key(), exists);
				if (exists) {//Si el hijo actual no existe
					const_iterator childCompMod = child.value().find("ComponentModifications");
					if (childCompMod != child.value().end())
						modifyComponents(childData, *childCompMod);

					const_iterator childComp = child.value().find("Components");
					if (childComp != child.value().end())
						addComponents(childData, *childComp);
				}
			}
	}
	else if (type != "gameobject") {
		printf("ERROR: se ha introducido un tipo no valido para la creacion del objeto %s.\n", gOD.getName().c_str());
		return  GameObjectData();
	}

	//Se cargan los nuevos componentes
	const_iterator components = data.find("Components");
	if (components != data.end())
		addComponents(gOD, *components);

	//Se cargan los datos de los hijos con una llamada recursiva
	const_iterator children = data.find("Children");
	if (children != data.end())
		for (auto& child : (*children).items()) {
			const_iterator name = child.value().find("ObjectName");
			if (name == child.value().end()) {
				printf("ERROR: No se ha podido encontrar el nombre del hijo del objeto %s.\n", gOD.name.c_str());
				return GameObjectData();
			}

			gOD.addChildrenData(child.value()["ObjectName"], loadGameObjectData(child.value()));
		}

	return gOD;
}

SceneData DataLoader::loadSceneData(const json& data)
{
	SceneData sD = SceneData();

	const_iterator name = data.find("SceneName");
	if (name == data.end()) {
		printf("ERROR: No se ha podido encontrar el nombre de la escena.\n");
		return sD;
	}
	sD.setName(*name);

	const_iterator objects = data.find("SceneObjects");
	if (objects != data.end())
		for (auto& gameObject : (*objects).items()) {
			sD.setGameObjectData(loadGameObjectData(gameObject.value()));
		}

	return sD;
}

void DataLoader::addComponents(GameObjectData& gOD, const json& data)
{
	for (auto& component : data.items()) {
		const_iterator name = component.value().find("ComponentName");
		if (name == component.value().end()) {
			printf("ERROR: No se ha podido encontrar el nombre del componente a añadir.\n");
			return;
		}

		gOD.addComponentData(*name, loadComponentData(component.value()));
	}
}

void DataLoader::modifyComponents(GameObjectData& gOD, const json& data)
{
	for (auto& component : data.items()) {
		const_iterator name = component.value().find("ComponentName");
		if (name == component.value().end()) {
			printf("ERROR: No se ha podido encontrar el nombre del componente a modificar.\n");
			return;
		}
		const_iterator properties = component.value().find("ComponentProperties");
		if (properties != component.value().end()) {
			for (auto& property : (*properties).items()) {
				if (!gOD.modifyComponentData(*name, property.key(), property.value()))//Si no se ha podido modificar el componente pasamos al siguiente
					break;
			}
		}
	}
}

