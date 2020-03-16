#include "GameObjectData.h"
#include "DebugUtils.h"
#include <stdio.h>

GameObjectData::GameObjectData()
{
}

GameObjectData::GameObjectData(const GameObjectData& other)
{
	name = other.name;
	tag = other.tag;

	for (auto c : other.componentData)
		componentData[c.first] = new ComponentData(*c.second);

	for (auto c : other.childrenData)
		childrenData[c.first] = new GameObjectData(*c.second);
}

GameObjectData::~GameObjectData()
{
	for (auto c : componentData) {
		delete c.second;
		c.second = nullptr;
	}
		
	for (auto c : childrenData) {
		delete c.second;
		c.second = nullptr;
	}

	componentData.clear();
	childrenData.clear();
}

void GameObjectData::setName(const std::string& gameObjectName)
{
	name = gameObjectName;
}

void GameObjectData::setTag(const std::string& gameObjectTag)
{
	tag = gameObjectTag;
}

void GameObjectData::addComponentData(const std::string& componentName, ComponentData* data)
{
	if (componentData.find(componentName) != componentData.end()) {
		LOG("GAMEOBJECT DATA: object %s component %s data has been overwritten\n", name.c_str(), componentName.c_str());
		delete componentData[componentName];
	}
	componentData[componentName] = data;
}

void GameObjectData::addChildrenData(const std::string& childrenName, GameObjectData* data)
{
	if (componentData.find(childrenName) != componentData.end()) {
		LOG("GAMEOBJECT DATA: object %s child %s data has been overwritten\n", name.c_str(), childrenName.c_str());
		delete childrenData[childrenName];
	}
	childrenData[childrenName] = data;
}

bool GameObjectData::modifyComponentData(const std::string& componentName, const std::string& propertyName, const std::string& value)
{
	if (componentData.find(componentName) == componentData.end()) {
		LOG("GAMEOBJECT DATA: tried to modify component %s, does not exist in object %s\n", componentName.c_str(), name.c_str());
		return false;
	}
	componentData[componentName]->modifyProperty(propertyName, value);
	return true;
}

bool GameObjectData::modifyChildData(const std::string& childName, const std::string& componentName, const std::string& propertyName, const std::string& value)
{
	if (childrenData.find(childName) == childrenData.end()) {
		LOG("GAMEOBJECT DATA: tried to modify child %s, does not exist in object %s\n", childName.c_str(), name.c_str());
		return false;
	}
	return childrenData[childName]->modifyComponentData(componentName, propertyName, value);
	
}

const std::string& GameObjectData::getName() const
{
	return name;
}

const std::string& GameObjectData::getTag() const
{
	return tag;
}

const std::unordered_map<std::string, ComponentData*>& GameObjectData::getComponentData() const
{
	return componentData;
}

const std::map<std::string, GameObjectData*>& GameObjectData::getChildrenData() const
{
	return childrenData;
}

GameObjectData* GameObjectData::getChild(const std::string& childName, bool& exists)
{
	auto child = childrenData.find(childName);
	if (child == childrenData.end()) {
		exists = false;
		LOG("GAMEOBJECT DATA: child with name %s does not exist in object %s\n", childName.c_str(), name.c_str());
		return GameObjectData::empty();
	}
	exists = true;
	return  (*child).second;
}

ComponentData* GameObjectData::getComponent(const std::string& componentName, bool& exists)
{
	auto component = componentData.find(componentName);
	if (component == componentData.end()) {
		exists = false;
		LOG("GAMEOBJECT DATA: component with name %s does not exist in object %s\n", componentName.c_str(), name.c_str());
		return ComponentData::empty();
	}
	exists = true;
	return  (*component).second;
}

GameObjectData* GameObjectData::empty()
{
	GameObjectData* g = new GameObjectData();
	g->name = g->tag = "null";
	return g;
}
