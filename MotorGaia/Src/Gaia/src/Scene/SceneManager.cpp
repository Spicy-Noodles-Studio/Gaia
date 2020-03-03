#include "SceneManager.h"
#include "ResourcesManager.h"
#include "ComponentManager.h"
#include "GameObject.h"
#include "SceneData.h"

SceneManager::SceneManager() : currentScene(nullptr), stackScene(nullptr), root(nullptr)
{
}

SceneManager::~SceneManager()
{
	close();
}

void SceneManager::init(Ogre::Root* root)
{
	this->root = root;
}

void SceneManager::close()
{
	if (currentScene != nullptr)
		delete currentScene;
	if (stackScene != nullptr)
		delete stackScene;
}

void SceneManager::preUpdate(float deltaTime)
{
	// If stack not empty, change scene and delete the current one
	if (stackScene != nullptr) {
		Scene* oldScene = currentScene;
		currentScene = stackScene;
		stackScene = nullptr;
		if(oldScene != nullptr)
			delete oldScene;
	}
}

void SceneManager::update(float deltaTime) {
	//All stuff about scene
	currentScene->awake();
	currentScene->start();
	currentScene->preUpdate(deltaTime);
	currentScene->update(deltaTime);
	currentScene->postUpdate(deltaTime);
}

void SceneManager::postUpdate(float deltaTime)
{
	// Stuff jaja
}

bool SceneManager::changeScene(const std::string& name, bool async)
{
	// Check if scene exists
	const SceneData* data = ResourcesManager::getSceneData(name);
	if (data == nullptr) {
		printf("SCENE MANAGER: scen with name %s not found\n", name.c_str());
		return false;
	}

	// Creates the Scene by its data (assuming creation was succesfull)
	stackScene = createScene(data);

	return true;
}

bool SceneManager::createScene(const std::string& name) {

	bool created = true;
	const SceneData* sD = ResourcesManager::getSceneData(name);
	if (sD != nullptr) {
		Scene* myScene = new Scene(name);
		for (GameObjectData* gameObjectD : sD->getGameObjectsData()) {
			GameObject* gameObject = new GameObject(gameObjectD->getName(), gameObjectD->getTag(), myScene);
			for (auto it : gameObjectD->getComponentData()) {
				ComponentData* cD = it.second;
				auto constructor = ComponentManager::getComponentFactory(cD->getName());
				if (constructor != nullptr) {
					Component* comp = constructor(gameObject);
					comp->handleData(cD);
					created = created && gameObject->addComponent(cD->getName(), comp);
				}
				else
					created = false;
			}
			created = created && myScene->addGameObject(gameObject);
		}
		//scenes[name] = myScene;
		return created;
	}
	else
		return false;
}

Scene* SceneManager::createScene(const SceneData* data)
{
	Scene* myScene = new Scene(data->name);
	for (GameObjectData* gameObjectD : data->getGameObjectsData()) {
		GameObject* gameObject = new GameObject(gameObjectD->getName(), gameObjectD->getTag(), myScene);
		for (auto it : gameObjectD->getComponentData()) {
			ComponentData* cD = it.second;
			auto constructor = ComponentManager::getComponentFactory(cD->getName());
			if (constructor != nullptr) {
				Component* comp = constructor(gameObject);
				comp->handleData(cD);
				gameObject->addComponent(cD->getName(), comp);
			}
		}
		myScene->addGameObject(gameObject);
	}
	return myScene;
}

bool SceneManager::exist(const std::string& name)
{
	return ResourcesManager::getSceneData(name) != nullptr;
}


