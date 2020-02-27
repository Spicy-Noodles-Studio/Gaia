#include "ResourcesManager.h"

std::map<std::string, SceneData*> ResourcesManager::sceneData;

ResourcesManager::ResourcesManager() : dataLoader(this)
{

}

ResourcesManager::~ResourcesManager()
{
	for (auto s : sceneData)
		delete s.second;
}

void ResourcesManager::init()
{
}

void ResourcesManager::clean()
{
}

void ResourcesManager::loadScene(const std::string& filename)
{
	if (!dataLoader.loadScene(filename))
		printf("RESOURCES MANAGER: invalid filename %s", filename.c_str());
}

void ResourcesManager::loadBlueprint(const std::string& filename)
{
}

void ResourcesManager::registerSceneData(SceneData* data)
{
	if (sceneData.find(data->name) != sceneData.end()) {
		printf("RESOURCES MANAGER: trying to add an already existing SceneData: %s\n", data->name.c_str());
		return;
	}
	sceneData[data->name] = data;
}

SceneData* ResourcesManager::getSceneData(const std::string& name)
{
	if (sceneData.find(name) == sceneData.end()) {
		printf("RESOURCES MANAGER: trying to get none existing SceneData: %s\n", name.c_str());
		return nullptr;
	}
	return sceneData[name];
}
