#pragma once
#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include <OgreRoot.h>
#include "GameObject.h"
#include "Camera.h"

#include "GaiaComponent.h"
#include "UserComponent.h"

class Scene {
	friend class SceneManager;
	friend class UserComponent;
public:
	Scene(const std::string& sceneName, Ogre::Root* root);
	~Scene();

	//Component Calls
	void awake();
	void start();
	void preUpdate(float deltaTime);
	void update(float deltaTime);
	void postUpdate(float deltaTime);
	
	const std::string& getName();
	Ogre::SceneManager* getSceneManager() const;

	Ogre::Entity* createEntity(const std::string& name);

	GameObject* findGameObjectWithName(const std::string& name);
	GameObject* findGameObjectWithTag(const std::string& tag);

	void setMainCamera(Camera* camera);
	Camera* getMainCamera() const;

private:
	void addUserComponent(UserComponent* component);

	bool addGameObject(GameObject* gameObject);
	bool delGameObjectWithName(const std::string& name);
	bool delGameObjectWithTag(const std::string& tag);

	void destroyPendingGameObjects();
	void destroyGameObject(GameObject* gameObject);

	void updateAllAnimations(float deltaTime);

private:
	const std::string name;
	Ogre::Root* root;
	Ogre::SceneManager* sceneManager;

	std::vector<UserComponent*> userComponents;

	std::vector<GameObject*> sceneObjects;
	std::vector<GameObject*> destroyQueue;

	Camera* mainCamera;
};

#endif