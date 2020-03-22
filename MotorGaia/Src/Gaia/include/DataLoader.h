#pragma once

#ifndef DATA_LOADER_H
#define DATA_LOADER_H

#include <nlohmann/json.hpp>

class SceneData;
class BlueprintData;


using json = nlohmann::json;

class DataLoader {
public:
	DataLoader();
	~DataLoader();

	SceneData* loadScene(const std::string& filename, bool& loaded);
	GameObjectData* loadBlueprint(const std::string& filename, bool& loaded);

	ComponentData* loadComponentData(const json& data, bool& loaded);
	GameObjectData* loadGameObjectData(const json& data, bool& loaded);
	SceneData* loadSceneData(const json& data, bool& loaded);

private:
	bool addComponents(GameObjectData& gOD, const json& data);
	bool modifyComponents(GameObjectData& gOD, const json& data);
};

#endif