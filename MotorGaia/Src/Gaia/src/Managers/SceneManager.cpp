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

SceneManager::SceneManager() :	currentScene(nullptr), stackScene(nullptr), root(nullptr), sceneManager(nullptr), countNodeIDs(0), timeScaleAccumulator(0.0f),
								debugDrawer(nullptr)
{

}

SceneManager::~SceneManager()
{

}

void SceneManager::init(Ogre::Root* root)
{
	checkNullAndBreak(root);

	this->root = root;
	this->sceneManager = root->createSceneManager();

#ifdef _DEBUG
	debugDrawer = new DebugDrawer(this->sceneManager);
	PhysicsSystem* physicsSystem = PhysicsSystem::GetInstance();
	checkNullAndBreak(physicsSystem);
	physicsSystem->setDebugDrawer(debugDrawer);
#endif 

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
	if(currentScene != nullptr)
		currentScene->updateAllAnimations(deltaTime);
}

void SceneManager::update(float deltaTime)
{
	Timer* timer = Timer::GetInstance();
	if(timer != nullptr)
		timeScaleAccumulator += timer->getTimeScale();

	checkNullAndBreak(currentScene);

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
	checkNullAndBreak(currentScene);
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
		if (data == nullptr || data->getLoadState() == Loadable::LoadState::INVALID) {
			LOG_ERROR("SCENE MANAGER", "Scene with name %s not found", name.c_str());
			return false;
		}
	} while (data->getLoadState() != Loadable::LoadState::READY);

	loadScene(data);

	return data == nullptr ? false : true;
}

Scene* SceneManager::createScene(const SceneData* data)
{
	checkNullAndBreak(data, nullptr);

	Scene* myScene = new Scene(data->name, this);
	// For each GameObjectData, create the gameObject
	for (GameObjectData* gData : data->getGameObjectsData()) {
		createGameObject(gData, myScene);
	}
	return myScene;
}

GameObject* SceneManager::createGameObject(const GameObjectData* data, Scene* scene, GameObject* parent)
{
	checkNullAndBreak(data, nullptr);
	checkNullAndBreak(scene, nullptr);

	GameObjectData gData(*data);
	GameObject* gameObject = new GameObject(gData.getName(), gData.getTag(), scene);
	scene->addGameObject(gameObject);

	if (parent != nullptr)
		parent->addChild(gameObject);

	// Component
	for (auto compData : gData.getComponentData()) {
		ComponentData* cData = compData;
		if (cData == nullptr) continue;
		ComponentManager* componentManager = ComponentManager::GetInstance();
		if (componentManager == nullptr) {
			LOG_ERROR("SCENE MANAGER", "Error ocurred while creating a GameObject named \"%s\"", gameObject->getName().c_str());
			delete gameObject;
			return nullptr;
		}
		auto constructor = componentManager->getComponentFactory(cData->getName());
		if (constructor != nullptr)
		{
			Component* comp = constructor(gameObject);
			if (comp == nullptr) continue;

			comp->handleData(cData);
			if (!gameObject->addComponent(cData->getName(), comp))
				delete comp;
		}
	}

	// For each child, create the child
	for (auto childData : gData.getChildrenData()) {
		if (childData == nullptr) continue;
		GameObject* child = createGameObject(childData, scene, gameObject);
	}

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

	InterfaceSystem* interfaceSystem = InterfaceSystem::GetInstance();
	if (interfaceSystem != nullptr) {
		interfaceSystem->clearControllerMenuInput();
		interfaceSystem->initControllerMenuInput();
	}
}

void SceneManager::processCameraChange()
{
	checkNullAndBreak(currentScene);
	Camera* camera = currentScene->getMainCamera();
	checkNullAndBreak(camera);
	WindowManager* windowManager = WindowManager::GetInstance();
	checkNullAndBreak(windowManager);

	if (camera == nullptr)
	{
		LOG_ERROR("SCENE MANAGER", "Changing to scene \"%s\" that has no main camera", currentScene->getName().c_str());
		windowManager->removeAllViewportsFromWindow();
		return;
	}
	windowManager->removeAllViewportsFromWindow();
	Viewport* viewport = windowManager->addViewportToWindow(camera);

	RenderSystem* renderSystem = RenderSystem::GetInstance();
	checkNullAndBreak(renderSystem);

	renderSystem->applyBrightness(viewport);
}

void SceneManager::processDontDestroyObjects()
{
	if (currentScene == nullptr)
		return;

	for (GameObject* gameObject : currentScene->dontDestroyObjects) {
		if (gameObject == nullptr) continue;
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

bool SceneManager::exist(const std::string& name) const
{
	return ResourcesManager::getSceneData(name) != nullptr;
}


Scene* SceneManager::getCurrentScene() const
{
	return currentScene;
}

std::string SceneManager::getSceneToLoad() const
{
	return sceneToLoad;
}

