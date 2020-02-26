#pragma once

#include <vector>

#include "GameObject.h"

class Scene
{
public:
	Scene(std::string sceneName);
	~Scene();

	inline std::string getSceneName();

private:
	bool addGameObject(GameObject* gameObject);
	bool delGameObjectWithName(std::string name);
	bool delGameObjectWithTag(std::string tag);

	GameObject* findGameObjectWithName(std::string name);
	GameObject* findGameObjectWithTag(std::string tag);

private:
	std::string name;
	std::vector<GameObject*> sceneObjects;

};