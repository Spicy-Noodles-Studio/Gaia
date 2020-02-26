#pragma once

#ifndef GAMEOBJECT_DATA_H
#define GAMEOBJECT_DATA_H

#include <map>
#include <string>
#include "ComponentData.h"

class GameObjectData
{
public:
	GameObjectData();
	~GameObjectData();

	void setName(const std::string& gameObjectName);
	void setTag(const std::string& gameObjectTag);
	void addComponentData(const std::string& componentName, const ComponentData& data);
	void addChildrenData(const std::string& childrenName, const GameObjectData& data);

	bool modifyComponentData(const std::string& componentName, const std::string& propertyName, const std::string& value);
	bool modifyChildData(const std::string& childName, const std::string& componentName, const std::string& propertyName, const std::string& value);

	const std::string& getName() const;
	const std::string& getTag() const;
	const std::map<std::string, ComponentData>& getComponentData() const;
	const std::map<std::string, GameObjectData>& getChildrenData() const;
	GameObjectData& getChild(const std::string& childName, bool& exists);
	ComponentData& getComponent(const std::string& componentName, bool& exists);

public:
	std::string name;
	std::string tag;

	std::map<std::string, ComponentData> componentData;
	std::map<std::string, GameObjectData> childrenData;
};

#endif