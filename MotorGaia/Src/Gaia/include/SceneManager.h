#pragma once

#ifndef SCENE_MANAGER_H
#define SCENE_MANAGER_H

#include <string>
#include <map>

#include "Scene.h"
#include "SceneData.h"

#include <OgreRoot.h>


class SceneManager
{
public:
	SceneManager();
	~SceneManager();

	void init(Ogre::Root* root);
	void close();

	// Stuff before process currentScene
	void preUpdate(float deltaTime);
	// Process currentScene
	void update(float deltaTime);
	// Stuff after process currentScene
	void postUpdate(float deltaTime);

	bool changeScene(const std::string& name, bool async = false);
	bool exist(const std::string& name);

	Scene* getCurrentScene();

private:
	bool createScene(const std::string& name);
	Scene* createScene(const SceneData* data);

private:
	Scene* currentScene;
	Scene* stackScene;

	Ogre::Root* root;

};

#endif