#include "SceneData.h"

SceneData::SceneData()
{
}

SceneData::~SceneData()
{
}

inline void SceneData::setName(const std::string& sceneName)
{
	name = sceneName;
}

inline void SceneData::setGameObjectData(const GameObjectData& data)
{
	gData.push_back(data);
}

inline const std::string& SceneData::getName() const
{
	return name;
}

inline const std::vector<GameObjectData>& SceneData::getGameObjectsData() const
{
	return gData;
}
