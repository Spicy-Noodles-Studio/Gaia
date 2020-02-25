#pragma once

#ifndef SCENE_DATA_H
#define SCENE_DATA_H

#include "GameObjectData.h"

struct SceneData {
public:
	SceneData();
	~SceneData();

	inline void setName(const std::string& sceneName);
	inline void setGameObjectData(const GameObjectData& data);

	inline const std::string& getName() const;
	inline const std::vector<GameObjectData>& getGameObjectsData() const;

public:
	std::string name;
	std::vector<GameObjectData> gData;
};

#endif