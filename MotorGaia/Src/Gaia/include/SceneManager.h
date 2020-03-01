#pragma once

#ifndef SCENE_MANAGER_H
#define SCENE_MANAGER_H

#include <string>
#include <map>

#include "Scene.h"
#include "SceneData.h"


class SceneManager
{
public:
	SceneManager();
	~SceneManager();

public:

	bool loadScene(std::string name);
	bool exist(std::string name);

	// void addSceneData(const SceneData* sData);

private:
	// std::map<std::string, SceneData> sceneData;
	Scene* currentScene;
};

#endif