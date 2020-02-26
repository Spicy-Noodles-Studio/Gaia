#pragma once

#ifndef DATA_LOADER_H
#define DATA_LOADER_H

#include <nlohmann/json.hpp>

class ComponentData;
class GameObjectData;
class SceneData;

using json = nlohmann::json;

class DataLoader {
public:
	DataLoader();
	~DataLoader();

	ComponentData loadComponentData(const json& data);
	GameObjectData loadGameObjectData(const json& data);
	SceneData loadSceneData(const json& data);

private:
	void addComponents(GameObjectData& gOD, const json& data);
	void modifyComponents(GameObjectData& gOD, const json& data);
};

#endif