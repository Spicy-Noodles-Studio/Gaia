#include "SceneManager.h"

SceneManager::SceneManager() : currentScene(nullptr)
{
}

SceneManager::~SceneManager()
{
}

bool SceneManager::loadScene(std::string name)
{
	return false;
}

bool SceneManager::exist(std::string name)
{
	return false;
}

/*void SceneManager::addSceneData(const SceneData* sData)
{
	if (sceneData.find(sData->name) != sceneData.end())
	{
		printf("SCENE MANAGER: trying to add a scene with name %s. There is already an existing one\n", sData->name.c_str());
		return;
	}
	sceneData[sData->name] = *sData;
}*/
