#include "ResourcesManager.h"
#include <fstream>

std::map<std::string, SceneData*> ResourcesManager::sceneData;
std::map<std::string, GameObjectData*> ResourcesManager::blueprints;

ResourcesManager::ResourcesManager(const std::string& filePath) : dataLoader(), resourcesPath(filePath)
{

}

ResourcesManager::~ResourcesManager()
{
	for (auto s : sceneData)
		delete s.second;
	for (auto b : blueprints)
		delete b.second;

	sceneData.clear();
	blueprints.clear();
}


void ResourcesManager::init()
{
	// Pair: (Types, Filepath)
	std::vector<std::pair<std::string, std::string>> filePaths;
	// Reads all file paths
	std::fstream file(resourcesPath);
	if (!file.is_open()) {
		printf("RESOURCES MANAGER: Resources filepath not valid: %s\n", resourcesPath.c_str());
		return;
	}
	printf("Reading Resources paths...\n");
	// Start reading resources
	std::string type, c, filename;
	while (file >> type >> c >> filename) {
		if (c != "=") {
			printf("RESOURCES MANAGER: invalid resources file format\nFailed at: %s %s %s\n", type.c_str(), c.c_str(), filename.c_str());
			file.close();
			return;
		}
		filePaths.push_back({ type, filename });
		// Reset variables
		type = c = filename = "";
	}
	file.close();

	// Start loading resources
	for (int i = 0; i < filePaths.size(); i++) {
		std::string type = filePaths[i].first;
		std::string path = filePaths[i].second;
		loadResources(type, path);
	}
}

void ResourcesManager::clean()
{
}

void ResourcesManager::loadScene(const std::string& filename)
{
	bool loaded = true;
	registerSceneData(dataLoader.loadScene(filename, loaded));
	if (!loaded)
		printf("RESOURCES MANAGER: invalid Scene, filename %s.\n", filename.c_str());
}

void ResourcesManager::loadBlueprint(const std::string& filename)
{
	bool loaded = true;
	registerBlueprint(dataLoader.loadBlueprint(filename, loaded));
	if (!loaded)
		printf("RESOURCES MANAGER: invalid Blueprint, filename %s.\n", filename.c_str());
}

void ResourcesManager::registerSceneData(SceneData* data)
{
	if (sceneData.find(data->name) != sceneData.end()) {
		printf("RESOURCES MANAGER: trying to add an already existing SceneData: %s.\n", data->name.c_str());
		return;
	}
	sceneData[data->name] = data;
}

void ResourcesManager::registerBlueprint(GameObjectData* data)
{
	if (blueprints.find(data->name) != blueprints.end()) {
		printf("RESOURCES MANAGER: trying to add an already existing Blueprint: %s.\n", data->name.c_str());
		return;
	}
	blueprints[data->name] = data;
}

SceneData* ResourcesManager::getSceneData(const std::string& name)
{
	if (sceneData.find(name) == sceneData.end()) {
		printf("RESOURCES MANAGER: trying to get not existing SceneData: %s.\n", name.c_str());
		return nullptr;
	}
	return sceneData[name];
}

GameObjectData* ResourcesManager::getBlueprint(const std::string& name)
{
	if (blueprints.find(name) == blueprints.end()) {
		printf("RESOURCES MANAGER: trying to get not existing Blueprint: %s.\n", name.c_str());
		return nullptr;
	}
	return blueprints[name];
}

void ResourcesManager::loadResources(const std::string& resourceType, const std::string& path)
{
	if (resourceType == "Scenes")
		loadScenes(path);
	else if (resourceType == "Blueprints")
		loadBlueprints(path);
	else
		printf("RESOURCES MANAGER: invalid resource type: %s. Resource not loaded.\n", resourceType.c_str());
}

void ResourcesManager::loadScenes(const std::string& filename)
{
	std::fstream file(filename);
	if (!file.is_open()) {
		printf("RESOURCES MANAGER: ScenesAssets path %s not found.\n", filename.c_str());
		return;
	}
	std::vector<std::string> paths;
	std::string f;
	while (file >> f) paths.push_back(f);
	file.close();

	for (std::string pName : paths)
		loadScene(pName);
}

void ResourcesManager::loadBlueprints(const std::string& filename)
{
	std::fstream file(filename);
	if (!file.is_open()) {
		printf("RESOURCES MANAGER: BlueprintsAssets path %s not found.\n", filename.c_str());
		return;
	}
	std::vector<std::string> paths;
	std::string f;
	while (file >> f) paths.push_back(f);
	file.close();

	for (std::string pName : paths)
		loadBlueprint(pName);
}
