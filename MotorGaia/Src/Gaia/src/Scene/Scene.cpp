#include "Scene.h"
#include <OgreRoot.h>
#include "PhysicsSystem.h"
#include "GameObject.h"
#include "Camera.h"
#include "DebugDrawer.h"
#include "SceneManager.h"

Scene::Scene(const std::string& sceneName, SceneManager* sceneManager) : name(sceneName), sceneManager(sceneManager), mainCamera(nullptr), timeAccumulator(0.0f)
{

}

Scene::~Scene()
{
	for (GameObject* gameObject : sceneObjects) 
	{
		if (gameObject == nullptr) continue;
		//If it has to be destroyed
		if (std::find(dontDestroyObjects.begin(), dontDestroyObjects.end(), gameObject) != dontDestroyObjects.end())
			continue;

		delete gameObject;
		gameObject = nullptr;
	}

	for (GameObject* gameObject : instantiateQueue) 
	{
		if (gameObject == nullptr) continue;
		delete gameObject;
		gameObject = nullptr;
	}

	for (GameObject* gameObject : dontDestroyObjects)
	{
		if (gameObject == nullptr) continue;
		if (gameObject->myScene == this) {
			delete gameObject;
			gameObject = nullptr;
		}
	}

	sceneObjects.clear();
	destroyQueue.clear();
	dontDestroyObjects.clear();
	instantiateQueue.clear();
	animationSets.clear();

	mainCamera = nullptr;
}

void Scene::awake()
{
	std::vector<UserComponent*> userComponents = this->userComponents;

	// awake components
	for (UserComponent* component : userComponents) {
		if (component == nullptr) continue;
		if (component->isActive() && component->isSleeping()) {
			component->awake();
			component->sleeping = false;
		}
	}
}

void Scene::start()
{
	std::vector<UserComponent*> userComponents = this->userComponents;

	// start components
	for (UserComponent* component : userComponents) {
		if (component == nullptr) continue;
		if (component->isActive() && !component->isSleeping() && !component->hasStarted()) {
			component->start();
			component->started = true;
		}
	}
}

void Scene::preUpdate(float deltaTime)
{
	std::vector<UserComponent*> userComponents = this->userComponents;

	//Preupdate components
	for (UserComponent* component : userComponents) {
		if (component == nullptr) continue;
		if (component->isActive() && !component->isSleeping() && component->hasStarted())
			component->preUpdate(deltaTime);
	}

}

void Scene::update(float deltaTime)
{
	std::vector<UserComponent*> userComponents = this->userComponents;

	// update components
	for (UserComponent* component : userComponents) {
		if (component == nullptr) continue;
		if (component->isActive() && !component->isSleeping() && component->hasStarted())
			component->update(deltaTime);
	}
}

void Scene::postUpdate(float deltaTime)
{
	std::vector<UserComponent*> userComponents = this->userComponents;

	// postUpdate compoenent
	for (UserComponent* component : userComponents) {
		if (component == nullptr) continue;
		if (component->isActive() && !component->isSleeping() && component->hasStarted())
			component->postUpdate(deltaTime);
	}
}

void Scene::fixedUpdate(float deltaTime)
{
	timeAccumulator += deltaTime;
	while (timeAccumulator >= 1.0f / 50.0f)
	{
		std::vector<UserComponent*> userComponents = this->userComponents;

		for (UserComponent* component : userComponents) {
			if (component == nullptr) continue;
			if (component->isActive() && !component->isSleeping() && component->hasStarted())
				component->fixedUpdate(deltaTime);
		}
		timeAccumulator -= 1.0f / 50.0f;
	}
}

const std::string& Scene::getName() const
{
	return name;
}

Ogre::SceneManager* Scene::getSceneManager() const
{
	checkNullAndBreak(sceneManager, nullptr);
	checkNullAndBreak(sceneManager->sceneManager, nullptr);

	return sceneManager->sceneManager;
}

Ogre::Entity* Scene::createEntity(const std::string& name)
{
	Ogre::SceneManager* sceneManager = getSceneManager();
	checkNullAndBreak(sceneManager, nullptr);

	return sceneManager->createEntity(name);
}

GameObject* Scene::getGameObjectWithName(const std::string& name)
{
	for (GameObject* gameObject : sceneObjects) {
		if (gameObject == nullptr) continue;
		if (gameObject->getName() == name)
			return gameObject;
	}
	return nullptr;
}

std::vector<GameObject*> Scene::getGameObjectsWithTag(const std::string& tag)
{
	std::vector<GameObject*> result;

	for (GameObject* gameObject : sceneObjects) {
		if (gameObject == nullptr) continue;
		if (gameObject->getTag() == tag) {
			result.push_back(gameObject);
		}
	}

	return result;
}

void Scene::setMainCamera(Camera* camera)
{
	mainCamera = camera;
}

Camera* Scene::getMainCamera() const
{
	return mainCamera;
}

void Scene::addAnimationSet(const std::string& id, Ogre::AnimationStateSet* anims)
{
	animationSets[id] = anims;
}

void Scene::addUserComponent(UserComponent* component)
{
	userComponents.push_back(component);
}

bool Scene::addGameObject(GameObject* gameObject)
{
	std::string objectName = gameObject->getName();
	if (repeatedNames.find(objectName) != repeatedNames.end()) {
		objectName += ("(" + std::to_string(++repeatedNames[objectName]) + ")");
		LOG_ERROR("SCENE", "Trying to add gameobject with name %s that already exists in scene %s", gameObject->getName().c_str(), name.c_str());
		LOG_ERROR("SCENE", "Adding gameobject with name %s", objectName.c_str());
		gameObject->name = objectName;
		// Try to add again
		return addGameObject(gameObject);
	}

	repeatedNames[gameObject->getName()] = 0;
	sceneObjects.push_back(gameObject);
	gameObject->myScene = this;
	return true;
}

void Scene::destroyPendingGameObjects()
{
	if (!destroyQueue.size()) return;

	for (GameObject* gameObject : destroyQueue) {
		// Sacarlo de almacenamiento
		std::vector<GameObject*>::iterator goIt = std::find(sceneObjects.begin(), sceneObjects.end(), gameObject);

		for (auto component : gameObject->userComponents) {
			auto it = std::find(userComponents.begin(), userComponents.end(), component);
			userComponents.erase(it);
		}
		
		sceneObjects.erase(goIt);

		delete gameObject;
		gameObject = nullptr;
	}
	destroyQueue.clear();
}

void Scene::destroyGameObject(GameObject* gameObject)
{
	destroyQueue.push_back(gameObject);
	
	//Mira si esta en dontDestroy para sacarlo
	if (dontDestroyObjects.find(gameObject) != dontDestroyObjects.end())
		dontDestroyObjects.erase(gameObject);
}

void Scene::instantiate(GameObject* gameObject)
{
	//Comprueba nombres antes de meter en la cola de intanciacion
	std::string objectName = gameObject->getName();
	if (repeatedNames.find(objectName) != repeatedNames.end()) {
		objectName += ("(" + std::to_string(++repeatedNames[objectName]) + ")");
		LOG_ERROR("SCENE", "Trying to add gameobject with name %s that already exists in scene %s", gameObject->getName().c_str(), name.c_str());
		LOG_ERROR("SCENE", "Adding gameobject with name %s", objectName.c_str());
		gameObject->name = objectName;
		// Try to add again
		instantiate(gameObject);
		return;
	}

	repeatedNames[gameObject->getName()] = 0;

	instantiateQueue.push_back(gameObject);
}

void Scene::instantiatePendingGameObjects()
{
	if (!instantiateQueue.size()) return;

	for (auto gameObject : instantiateQueue) {
		gameObject->myScene = this;
		sceneObjects.push_back(gameObject);
	}
	instantiateQueue.clear();
}


void Scene::updateAllAnimations(float deltaTime)
{
	for (auto set : animationSets)
	{
		for (auto anim : set.second->getEnabledAnimationStates())
			anim->addTime(deltaTime);
	}
}

void Scene::dontDestroyOnLoad(GameObject* gameObject)
{
	// Dont register an already existing object
	if (dontDestroyObjects.find(gameObject) != dontDestroyObjects.end())
		return;
	//Check if it is on the destroy queue
	if (std::find(destroyQueue.begin(), destroyQueue.end(), gameObject) != destroyQueue.end())
		return;

	dontDestroyObjects.insert(gameObject);
}