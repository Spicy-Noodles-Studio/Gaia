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

	bool createScene(std::string name);
	bool exist(std::string name);

private:
	Scene* currentScene;
	std::map<std::string, Scene*> scenes;
};

#endif