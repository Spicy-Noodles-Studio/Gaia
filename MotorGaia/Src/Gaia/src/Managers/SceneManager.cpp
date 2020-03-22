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

}

void SceneManager::init(Ogre::Root* root, Window* window)
{
	this->root = root;
	this->window = window;

	loadScene(ResourcesManager::getSceneData(0));
	// Let it change runtime
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

	destroy();
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
	currentScene->instantiatePendingGameObjects();
}

bool SceneManager::changeScene(const std::string& name, bool async)
{
	// Check if scene exists
	const SceneData* data = nullptr;
	do {
		data = ResourcesManager::getSceneData(name);
		if (data == nullptr) {
			LOG_ERROR("SCENE MANAGER", "scene with name %s not found", name.c_str());
			return false;
		}
	} while (data->getLoadState() != Loadable::LoadState::READY);

	loadScene(data);

	return data == nullptr ? false : true;
}

Scene* SceneManager::createScene(const SceneData* data)
{
	Scene* myScene = new Scene(data->name, root);
	// For each GameObjectData, create the gameObject
	for (GameObjectData* gData : data->getGameObjectsData()) {
		createGameObject(gData, myScene);
	}
	return myScene;
}

GameObject* SceneManager::createGameObject(const GameObjectData* data, Scene* scene, GameObject* parent)
{
	GameObject* gameObject = nullptr;
	// Check if it is a blueprint
	if (data->blueprintRef != nullptr) {
		GameObjectData bpData(*data->blueprintRef);
		bpData.name = data->name;
		bpData.tag = data->tag;

		//Children mods
		for (auto childMod : data->childrenModifications) {
			bpData.applyChildModification(childMod.first, childMod.second);
		}
		//Component mods
		for (auto compMod : data->componentModifications) {
			bpData.applyComponentModification(compMod.first, compMod.second);
		}
		
		gameObject = createGameObject(&bpData, scene, parent);
	}
	else {
		gameObject = new GameObject(data->getName(), data->getTag(), scene);
		scene->addGameObject(gameObject);
	}

	if (parent != nullptr)
		parent->addChild(gameObject);

	// For each child, create the child
	for (auto childData : data->getChildrenData()) {
		GameObject* child = createGameObject(childData.second, scene, gameObject);
	}

	// Component
	for (auto compData : data->getComponentData()) {
		ComponentData* cData = compData.second;
		auto constructor = ComponentManager::GetInstance()->getComponentFactory(cData->getName());
		if (constructor != nullptr)
		{
			Component* comp = constructor(gameObject);
			comp->handleData(cData);
			if (!gameObject->addComponent(cData->getName(), comp))
				delete comp;
		}
	}


	return gameObject;
}

void SceneManager::loadScene(const SceneData* data)
{
	if (data == nullptr) {
		LOG("SCENE MANAGER: given SceneData not valid. Loading default SceneData\n");
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
		LOG("SCENE MANAGER: changing to scene \"%s\" that has no main camera\n", currentScene->getName().c_str());
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


