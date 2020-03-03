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

	bool createScene(const std::string& name);
	bool exist(const std::string& name);

private:
	Scene* currentScene;
	std::map<std::string, Scene*> scenes;
};

#endif