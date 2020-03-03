#pragma once
#ifndef SCENE_H
#define SCENE_H

#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include "GameObject.h"

class Scene
{
public:
	Scene(std::string sceneName);
	~Scene();

	bool addGameObject(GameObject* gameObject);

private:
	bool delGameObjectWithName(const std::string& name);
	bool delGameObjectWithTag(const std::string& tag);

	GameObject* findGameObjectWithName(const std::string& name);
	GameObject* findGameObjectWithTag(const std::string& tag);

public:
	inline std::string getSceneName();

private:
	std::string name;
	std::vector<GameObject*> sceneObjects;

};

#endif