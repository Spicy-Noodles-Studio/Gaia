#include "GameObjectData.h"

#include <stdio.h>

GameObjectData::GameObjectData()
{
}

GameObjectData::~GameObjectData()
{
}

void GameObjectData::setName(const std::string& gameObjectName)
{
	name = gameObjectName;
}

void GameObjectData::setTag(const std::string& gameObjectTag)
{
	tag = gameObjectTag;
}

void GameObjectData::addComponentData(const std::string& componentName, const ComponentData& data)
{
	if (componentData.find(componentName) != componentData.end())
		printf("ADVERTENCIA: se han sobreescrito los datos del componente %s del objeto %s.\n", componentName.c_str(), name.c_str());
	componentData[componentName] = data;
}

void GameObjectData::addChildrenData(const std::string& childrenName, const GameObjectData& data)
{
	if (componentData.find(childrenName) != componentData.end())
		printf("ADVERTENCIA: se han sobreescrito los datos del hijo %s del objeto %s.\n", childrenName.c_str(), name.c_str());
	childrenData[childrenName] = data;
}

bool GameObjectData::modifyComponentData(const std::string& componentName, const std::string& propertyName, const std::string& value)
{
	if (componentData.find(componentName) == componentData.end()) {
		printf("ERROR: se ha intentado modificar el componente %s, que no existe en el objeto %s.\n", componentName.c_str(), name.c_str());
		return false;
	}
	componentData[componentName].modifyProperty(propertyName, value);
	return true;
}

bool GameObjectData::modifyChildData(const std::string& childName, const std::string& componentName, const std::string& propertyName, const std::string& value)
{
	if (childrenData.find(childName) == childrenData.end()) {
		printf("ERROR: se ha intentado modificar el hijo %s, que no existe en el objeto %s.\n", childName.c_str(), name.c_str());
		return false;
	}
	return childrenData[childName].modifyComponentData(componentName, propertyName, value);
	
}

const std::string& GameObjectData::getName() const
{
	return name;
}

const std::string& GameObjectData::getTag() const
{
	return tag;
}

const std::map<std::string, ComponentData>& GameObjectData::getComponentData() const
{
	return componentData;
}

const std::map<std::string, GameObjectData>& GameObjectData::getChildrenData() const
{
	return childrenData;
}

GameObjectData& GameObjectData::getChild(const std::string& childName, bool& exists)
{
	GameObjectData empty = GameObjectData();
	auto child = childrenData.find(childName);
	if (child == childrenData.end()) {
		exists = false;
		printf("ERROR: El hijo con nombre %s no existe en el objeto %s.\n", childName.c_str(), name.c_str());
		return empty;//TODO: return the default gameobject data
	}
	exists = true;
	return  (*child).second;
}

ComponentData& GameObjectData::getComponent(const std::string& componentName, bool& exists)
{
	ComponentData empty = ComponentData();
	auto component = componentData.find(componentName);
	if (component == componentData.end()) {
		exists = false;
		printf("ERROR: El componente con nombre %s no existe en el objeto %s.\n", componentName.c_str(), name.c_str());
		return empty;//TODO: return the default component data
	}
	exists = true;
	return  (*component).second;
}
