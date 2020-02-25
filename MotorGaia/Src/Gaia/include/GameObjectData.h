#pragma once

#ifndef GAMEOBJECT_DATA_H
#define GAMEOBJECT_DATA_H

#include <vector>
#include "ComponentData.h"

class GameObjectData
{
public:
	GameObjectData();
	~GameObjectData();

	inline void setName(const std::string& gameObjectName);
	inline void setTag(const std::string& gameObjectTag);
	inline void setComponentData(const ComponentData& data);

	inline const std::string& getName() const;
	inline const std::string& getTag() const;
	inline const std::vector<ComponentData>& getComponentData() const;

public:
	std::string name;
	std::string tag;

	std::vector<ComponentData> componentData;
};

#endif