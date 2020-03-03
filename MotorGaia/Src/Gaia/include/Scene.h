#pragma once

#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include "GameObject.h"

#include "GaiaComponent.h"
#include "UserComponent.h"

class Scene {
	friend class SceneManager;
public:
	Scene(const std::string& sceneName);
	~Scene();

	void awake();
	void start();
	void preUpdate(float deltaTime);
	void update(float deltaTime);
	void postUpdate(float deltaTime);
	
	const std::string& getName();

private:
	bool addGameObject(GameObject* gameObject);
	bool delGameObjectWithName(const std::string& name);
	bool delGameObjectWithTag(const std::string& tag);

	GameObject* findGameObjectWithName(const std::string& name);
	GameObject* findGameObjectWithTag(const std::string& tag);


private:
	const std::string name;
	// Aun por ver
	std::vector<GameObject*> sceneObjects;

	std::vector<GaiaComponent*> gaiaComponents;
	std::vector<UserComponent*> userComponents;

};

#endif