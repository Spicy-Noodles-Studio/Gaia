#pragma once

#ifndef GAMEOBJECT_DATA_H
#define GAMEOBJECT_DATA_H

#include <unordered_map>
#include <string>
#include "ComponentData.h"
#include <nlohmann\json.hpp>


typedef nlohmann::json RawData;
class BlueprintData;

class GAIA_API GameObjectData
{
public:
	GameObjectData();
	GameObjectData(const GameObjectData& other);
	~GameObjectData();

	bool loadData(const RawData& data);
	bool addComponentModifications(const RawData& data);
	bool addChildrenModifications(const RawData& data);
	bool addComponents(const RawData& data);
	bool addComponent(const RawData& data);
	bool addChildren(const RawData& data);
	bool addChild(const RawData& data);

	void setName(const std::string& gameObjectName);
	void setTag(const std::string& gameObjectTag);
	void addComponentData(const std::string& componentName, ComponentData* data);
	void addChildrenData(const std::string& childrenName, GameObjectData* data);

	void setBlueprint(const BlueprintData* bpRef);

	void applyChildModification(const std::string& name, GameObjectData* data);
	void applyComponentModification(const std::string& name, ComponentData* data);
	bool modifyComponentData(const std::string& componentName, const std::string& propertyName, const std::string& value);
	bool modifyChildData(const std::string& childName, const std::string& componentName, const std::string& propertyName, const std::string& value);

	const std::string& getName() const;
	const std::string& getTag() const;
	const std::unordered_map<std::string, ComponentData*>& getComponentData() const;
	const std::unordered_map<std::string, GameObjectData*>& getChildrenData() const;
	GameObjectData* getChild(const std::string& childName, bool& exists);
	ComponentData* getComponent(const std::string& componentName, bool& exists);

	static GameObjectData* empty();

public:
	std::string name;
	std::string tag;
	const BlueprintData* blueprintRef;

	std::unordered_map<std::string, ComponentData*> components;
	std::unordered_map<std::string, GameObjectData*> children;

	std::unordered_map<std::string, ComponentData*> componentModifications;
	std::unordered_map<std::string, GameObjectData*> childrenModifications;

};

#endif