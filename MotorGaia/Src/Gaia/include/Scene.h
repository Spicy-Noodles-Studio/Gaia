#pragma once
#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include <map>
#include <set>
#include "UserComponent.h"

namespace Ogre {
	class Root;
	class SceneManager;
	class Entity;
	class AnimationStateSet;
}

class Camera;
class GameObject;
class SceneManager;

class GAIA_API Scene {
	friend class GameObject;
	friend class SceneManager;
	friend class UserComponent;
public:
	Scene(const std::string& sceneName, SceneManager* sceneManager);
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

	GameObject* getGameObjectWithName(const std::string& name);
	std::vector<GameObject*> getGameObjectsWithTag(const std::string& tag);

	void setMainCamera(Camera* camera);
	Camera* getMainCamera() const;

	void addAnimationSet(const std::string& id, Ogre::AnimationStateSet* anims);

private:
	void addUserComponent(UserComponent* component);

	bool addGameObject(GameObject* gameObject);

	void destroyPendingGameObjects();
	void destroyGameObject(GameObject* gameObject);

	void instantiatePendingGameObjects();
	void instantiate(GameObject* gameObject);

	void updateAllAnimations(float deltaTime);

	void dontDestroyOnLoad(GameObject* gameObject);

private:
	const std::string name;
	SceneManager* sceneManager;

	std::vector<UserComponent*> userComponents;

	/* Name - Number of instances */
	std::map<std::string, int> repeatedNames;

	/* Objects, no parent-child */
	std::vector<GameObject*> sceneObjects;
	std::vector<GameObject*> destroyQueue;
	std::vector<GameObject*> instantiateQueue;

	std::set<GameObject*> dontDestroyObjects;

	Camera* mainCamera;

	std::map<std::string, Ogre::AnimationStateSet*> animationSets;
};

#endif