#pragma once

#ifndef SCENE_MANAGER_H
#define SCENE_MANAGER_H

#include "Singleton.h"

#include <string>
#include <map>
#include <future>

#include "Scene.h"
#include "SceneData.h"

#include <OgreRoot.h>
#include "Camera.h"

class DebugDrawer;

class GAIA_API  SceneManager : public Singleton<SceneManager>
{
	friend class Scene;
	friend class GameObject;
public:
	SceneManager();
	~SceneManager();

	void init(Ogre::Root* root);
	void close();

	// Before process currentScene
	void preUpdate(float deltaTime);
	// Process currentScene
	void update(float deltaTime);
	// After process currentScene
	void postUpdate(float deltaTime);

	bool changeScene(const std::string& name, bool async = false);
	bool exist(const std::string& name);

	Scene* getCurrentScene();

private:
	Scene* createScene(const SceneData* data);
	GameObject* createGameObject(const GameObjectData* data, Scene* scene, GameObject* parent = nullptr);

	// For loading screens
	bool preloadLoadingScreen();
	bool changeSceneAsync(const std::string& name);
	void loadSceneAsync(const SceneData* data);

	void loadScene(const SceneData* data);

	void processSceneChange();
	void processCameraChange();
	void processDontDestroyObjects();

	std::string getNextNodeID();

private:
	Scene* currentScene;
	Scene* stackScene;

	// For loading screens
	Scene* loadingScreen;
	Scene* sceneToLoad;
	std::future<bool> loadingThread;
	bool dontDestroyObjectsLoaded;
	bool finishedLoading;

	Ogre::Root* root;
	Ogre::SceneManager* sceneManager;

	int countNodeIDs;

	DebugDrawer* debugDrawer;

	float timeScaleAccumulator;
};

#endif