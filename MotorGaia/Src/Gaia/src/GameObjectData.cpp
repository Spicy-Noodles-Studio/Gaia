#include "GameObjectData.h"

GameObjectData::GameObjectData()
{
}

GameObjectData::~GameObjectData()
{
}

inline void GameObjectData::setName(const std::string& gameObjectName)
{
	name = gameObjectName;
}

inline void GameObjectData::setTag(const std::string& gameObjectTag)
{
}

inline void GameObjectData::setComponentData(const ComponentData& data)
{
}

inline const std::string& GameObjectData::getName() const
{
	return name;
}

inline const std::string& GameObjectData::getTag() const
{
	return tag;
}

inline const std::vector<ComponentData>& GameObjectData::getComponentData() const
{
	return componentData;
}
