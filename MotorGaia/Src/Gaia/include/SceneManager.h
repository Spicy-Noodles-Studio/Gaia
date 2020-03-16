#pragma once

#ifndef SCENE_MANAGER_H
#define SCENE_MANAGER_H

#include "Singleton.h"

#include <string>
#include <map>

#include "Scene.h"
#include "SceneData.h"

#include <OgreRoot.h>
#include "Window.h"
#include "Camera.h"


class GAIA_API  SceneManager : public Singleton<SceneManager>
{
public:
	SceneManager();
	~SceneManager();

	void init(Ogre::Root* root, Window* window);
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

	void loadScene(const SceneData* data);

	void processSceneChange();
	void processCameraChange();

private:
	Scene* currentScene;
	Scene* stackScene;

	Ogre::Root* root;
	Window* window;
};

#endif