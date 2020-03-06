#include "SceneManager.h"
#include "ResourcesManager.h"
#include "ComponentManager.h"
#include "GameObject.h"
#include "SceneData.h"

SceneManager::SceneManager() : currentScene(nullptr), stackScene(nullptr), root(nullptr), window(nullptr)
{
}

SceneManager::~SceneManager()
{
	close();
}

void SceneManager::init(Ogre::Root* root, Window* window)
{
	this->root = root;
	this->window = window;

	loadScene(ResourcesManager::getSceneData(0));
	processSceneChange();
}

void SceneManager::close()
{
	if (currentScene != nullptr)
		delete currentScene;
	if (stackScene != nullptr)
		delete stackScene;

	currentScene = nullptr;
	stackScene = nullptr;
}

void SceneManager::preUpdate(float deltaTime)
{
	// If stack not empty, change scene and delete the current one
	processSceneChange();

	//Update all animations
	currentScene->updateAllAnimations(deltaTime);
}

void SceneManager::update(float deltaTime)
{
	//All stuff about scene
	currentScene->awake();
	currentScene->start();
	currentScene->preUpdate(deltaTime);
	currentScene->update(deltaTime);
	currentScene->postUpdate(deltaTime);
}

void SceneManager::postUpdate(float deltaTime)
{
	// Destroy pending object
	currentScene->destroyPendingGameObjects();
	// Instantiate pending objects
	// TODO: Instantiate stuff
}

bool SceneManager::changeScene(const std::string& name, bool async)
{
	// Check if scene exists
	const SceneData* data = ResourcesManager::getSceneData(name);
	if (data == nullptr)
		printf("SCENE MANAGER: scene with name %s not found\n", name.c_str());

	loadScene(data);

	return data == nullptr ? false : true;
}

Scene* SceneManager::createScene(const SceneData* data)
{
	Scene* myScene = new Scene(data->name, root);
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

void SceneManager::loadScene(const SceneData* data)
{
	if (data == nullptr) {
		printf("SCENE MANAGER: given SceneData not valid. Loading default SceneData\n");
		return;
	}
	// Creates the Scene by its data (assuming creation was succesfull)
	stackScene = createScene(data);
	
}

void SceneManager::processSceneChange()
{
	if (stackScene == nullptr)
		return;

	Scene* oldScene = currentScene;
	currentScene = stackScene;
	stackScene = nullptr;
	if (oldScene != nullptr)
		delete oldScene;

	processCameraChange();
}

void SceneManager::processCameraChange()
{
	Camera* camera = currentScene->getMainCamera();
	if (camera == nullptr)
	{
		printf("SCENE MANAGER: changing to scene \"%s\" that has no main camera\n", currentScene->getName().c_str());
		window->removeAllViewports();
		return;
	}
	window->removeAllViewports();
	window->addViewport(camera->getCamera());
}

bool SceneManager::exist(const std::string& name)
{
	return ResourcesManager::getSceneData(name) != nullptr;
}

Scene* SceneManager::getCurrentScene()
{
	return currentScene;
}


