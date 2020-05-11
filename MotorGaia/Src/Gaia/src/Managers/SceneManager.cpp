#include "SceneManager.h"
#include "ResourcesManager.h"
#include "ComponentManager.h"
#include "GameObject.h"
#include "SceneData.h"
#include "PhysicsSystem.h"
#include "RenderSystem.h"
#include "DebugDrawer.h"
#include "InterfaceSystem.h"
#include "WindowManager.h"
#include "Timer.h"

SceneManager::SceneManager() : currentScene(nullptr), stackScene(nullptr), root(nullptr), sceneManager(nullptr), countNodeIDs(0), debugDrawer(nullptr), timeScaleAccumulator(0.0f)
{

}

SceneManager::~SceneManager()
{

}

void SceneManager::init(Ogre::Root* root)
{
	this->root = root;
	this->sceneManager = root->createSceneManager();

	debugDrawer = new DebugDrawer(this->sceneManager);
	PhysicsSystem::GetInstance()->setDebugDrawer(debugDrawer);

	loadScene(ResourcesManager::getSceneData(0));
	// Let it change runtime
	processSceneChange();

	sceneToLoad = "NO SCENE";
}

void SceneManager::close()
{
	if (debugDrawer != nullptr)
		delete debugDrawer;
	if (currentScene != nullptr) 
		delete currentScene;
	if (stackScene != nullptr)
		delete stackScene;

	debugDrawer = nullptr;
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
	timeScaleAccumulator += Timer::GetInstance()->getTimeScale();
	//All stuff about scene
	currentScene->awake();
	currentScene->start();
	currentScene->preUpdate(deltaTime);
	while (timeScaleAccumulator >= 1)
	{
		currentScene->update(deltaTime);
		timeScaleAccumulator--;
	}
	currentScene->fixedUpdate(deltaTime);
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
	// Create Loading Scene if async set to "true"
	if (async) {
		sceneToLoad = name;
		return changeScene("LoadingScreen");
	}
	if (name == sceneToLoad) sceneToLoad = "NO SCENE";
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
	InterfaceSystem::GetInstance()->clearControllerMenuInput();

	return data == nullptr ? false : true;
}

Scene* SceneManager::createScene(const SceneData* data)
{
	Scene* myScene = new Scene(data->name, this);
	// For each GameObjectData, create the gameObject
	for (GameObjectData* gData : data->getGameObjectsData()) {
		createGameObject(gData, myScene);
	}
	return myScene;
}

GameObject* SceneManager::createGameObject(const GameObjectData* data, Scene* scene, GameObject* parent)
{
	GameObjectData gData(*data);
	GameObject* gameObject = new GameObject(gData.getName(), gData.getTag(), scene);
	scene->addGameObject(gameObject);

	if (parent != nullptr)
		parent->addChild(gameObject);

	// Component
	for (auto compData : gData.getComponentData()) {
		ComponentData* cData = compData;
		auto constructor = ComponentManager::GetInstance()->getComponentFactory(cData->getName());
		if (constructor != nullptr)
		{
			Component* comp = constructor(gameObject);
			comp->handleData(cData);
			if (!gameObject->addComponent(cData->getName(), comp))
				delete comp;
		}
	}

	// For each child, create the child
	for (auto childData : gData.getChildrenData())
		GameObject* child = createGameObject(childData, scene, gameObject);

	return gameObject;
}

void SceneManager::loadScene(const SceneData* data)
{
	if (data == nullptr) {
		LOG("SCENE MANAGER: given SceneData not valid. Loading default SceneData");
		SceneData* emptyScene = SceneData::empty();
		stackScene = createScene(emptyScene);
		delete emptyScene;
		return;
	}
	// Creates the Scene by its data (assuming creation was succesfull)
	stackScene = createScene(data);
}

void SceneManager::processSceneChange()
{
	if (stackScene == nullptr)
		return;

	processDontDestroyObjects();

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
		WindowManager::GetInstance()->removeAllViewportsFromWindow();
		
		return;
	}
	WindowManager::GetInstance()->removeAllViewportsFromWindow();
	Viewport* v= WindowManager::GetInstance()->addViewportToWindow(camera);

	RenderSystem::GetInstance()->applyBrightness(v);
}

void SceneManager::processDontDestroyObjects()
{
	if (currentScene == nullptr)
		return;

	for (GameObject* gameObject : currentScene->dontDestroyObjects) {
		//Reset components
		for (auto component : gameObject->userComponents) {
			component->sleeping = true;
			component->started = false;
			stackScene->addUserComponent(component);
		}
		stackScene->addGameObject(gameObject);
	}
}

std::string SceneManager::getNextNodeID()
{
	return std::string(std::to_string(countNodeIDs++));
}

bool SceneManager::exist(const std::string& name)
{
	return ResourcesManager::getSceneData(name) != nullptr;
}


Scene* SceneManager::getCurrentScene()
{
	return currentScene;
}

std::string SceneManager::getSceneToLoad()
{
	return sceneToLoad;
}

