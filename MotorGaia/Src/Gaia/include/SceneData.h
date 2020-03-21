#pragma once

#ifndef SCENE_DATA_H
#define SCENE_DATA_H

#include <vector>

#include "GameObjectData.h"
#include "BlueprintData.h"

class GAIA_API SceneData : public Loadable {
public:
	SceneData();
	~SceneData();

	void setName(const std::string& sceneName);
	void setGameObjectData(GameObjectData* data);

	const std::string& getName() const;
	const std::vector<GameObjectData*>& getGameObjectsData() const;

	static SceneData* empty();

protected:
	virtual bool load_internal();

public:
	std::string name;
	std::vector<GameObjectData*> gameObjectData;
	std::vector<BlueprintData*> blueprintData;
};

#endif