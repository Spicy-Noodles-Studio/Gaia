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
	ResourcesManager(const std::string& filePath);
	~ResourcesManager();

	void init();
	void clean();

	void registerSceneData(SceneData* data);

	static SceneData* getSceneData(const std::string& name);
	//static Blueprint* getBlueprint(const std::string& name);

private:
	void loadResources(const std::string& resourceType, const std::string& filename);
	
	void loadScenes(const std::string& filename);
	void loadBlueprints(const std::string& filename);

	void loadScene(const std::string& filename);
	void loadBlueprint(const std::string& filename);

private:
	std::string resourcesPath;
	DataLoader dataLoader;

	static std::map<std::string, SceneData*> sceneData;

};

#endif
