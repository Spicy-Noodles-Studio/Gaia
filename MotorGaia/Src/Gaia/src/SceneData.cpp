#include "SceneData.h"

SceneData::SceneData()
{
}

SceneData::~SceneData()
{
}

void SceneData::setName(const std::string& sceneName)
{
	name = sceneName;
}

void SceneData::setGameObjectData(const GameObjectData& data)
{
	gData.push_back(data);
}

const std::string& SceneData::getName() const
{
	return name;
}

const std::vector<GameObjectData>& SceneData::getGameObjectsData() const
{
	return gData;
}
