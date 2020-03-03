#pragma once
#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include "GameObject.h"

#include "GaiaComponent.h"
#include "UserComponent.h"

class Scene {
	friend class SceneManager;
	friend class UserComponent;
public:
	Scene(const std::string& sceneName);
	~Scene();

	//Component Calls
	void awake();
	void start();
	void preUpdate(float deltaTime);
	void update(float deltaTime);
	void postUpdate(float deltaTime);
	
	const std::string& getName();

private:
	void addUserComponent(UserComponent* component);

	bool addGameObject(GameObject* gameObject);
	bool delGameObjectWithName(const std::string& name);
	bool delGameObjectWithTag(const std::string& tag);

	GameObject* findGameObjectWithName(const std::string& name);
	GameObject* findGameObjectWithTag(const std::string& tag);

	void destroyPendingGameObjects();
	void destroyGameObject(GameObject* gameObject);

private:
	const std::string name;

	std::vector<UserComponent*> userComponents;

	std::vector<GameObject*> sceneObjects;
	std::vector<GameObject*> destroyQueue;
};

#endif