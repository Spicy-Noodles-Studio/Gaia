#pragma once

#ifndef DATA_LOADER_H
#define DATA_LOADER_H

#include <nlohmann/json.hpp>


class ResourcesManager;
class ComponentData;
class GameObjectData;
class SceneData;

using json = nlohmann::json;

class DataLoader {
public:
	DataLoader(ResourcesManager* resourcesManager);
	~DataLoader();

	bool loadScene(const std::string& filename);
	bool loadBlueprint(const std::string& filename);

	ComponentData* loadComponentData(const json& data);
	GameObjectData* loadGameObjectData(const json& data);
	SceneData* loadSceneData(const json& data);

private:
	void addComponents(GameObjectData& gOD, const json& data);
	void modifyComponents(GameObjectData& gOD, const json& data);

private:
	ResourcesManager* resourcesManager;
};

#endif