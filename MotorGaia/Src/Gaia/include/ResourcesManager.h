#pragma once

#ifndef RESOURCES_MANAGER_H
#define RESOURCES_MANAGER_H

#include <map>
#include "DataLoader.h""
#include "SceneData.h"
#include "GameObjectData.h"

class ResourcesManager
{
public:
	ResourcesManager();
	~ResourcesManager();

	void init();
	void clean();

	void loadScene(const std::string& filename);
	void loadBlueprint(const std::string& filename);

	void registerSceneData(SceneData* data);

	static SceneData* getSceneData(const std::string& name);
	//static Blueprint* getBlueprint(const std::string& name);

private:
	DataLoader dataLoader;

	static std::map<std::string, SceneData*> sceneData;

};

#endif
