#pragma once

#ifndef SCENE_DATA_H
#define SCENE_DATA_H

#include <vector>

#include "GameObjectData.h"

struct SceneData {
public:
	SceneData();
	~SceneData();

	void setName(const std::string& sceneName);
	void setGameObjectData(const GameObjectData& data);

	const std::string& getName() const;
	const std::vector<GameObjectData>& getGameObjectsData() const;

public:
	std::string name;
	std::vector<GameObjectData> gData;
};

#endif