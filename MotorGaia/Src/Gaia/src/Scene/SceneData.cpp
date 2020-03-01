#include "SceneData.h"

SceneData::SceneData()
{
}

SceneData::~SceneData()
{
	for (int i = 0; i < gData.size(); i++) {
		delete gData[i];
		gData[i] = nullptr;
	}
	gData.clear();
}

void SceneData::setName(const std::string& sceneName)
{
	name = sceneName;
}

void SceneData::setGameObjectData(GameObjectData* data)
{
	gData.push_back(data);
}

const std::string& SceneData::getName() const
{
	return name;
}

const std::vector<GameObjectData*>& SceneData::getGameObjectsData() const
{
	return gData;
}

SceneData* SceneData::empty()
{
	SceneData* s = new SceneData();
	s->name = "null";
	return s;
}
