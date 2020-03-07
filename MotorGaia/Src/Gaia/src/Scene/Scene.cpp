#include "Scene.h"

Scene::Scene(const std::string& sceneName, Ogre::Root* root) : name(sceneName), root(root), sceneManager(root->createSceneManager()), mainCamera(nullptr)
{

}


Scene::~Scene()
{
	for (GameObject* g : sceneObjects) {
		delete g;
		g = nullptr;
	}

	sceneObjects.clear();
	destroyQueue.clear();
	
	root->destroySceneManager(sceneManager);

	mainCamera = nullptr;
}


void Scene::awake()
{
	// awake components
	for (UserComponent* c : userComponents) {
		if (c->isActive() && c->isSleeping()) {
			c->awake();
			c->setSleeping(false);
		}
	}
}


void Scene::start()
{
	// start components
	for (UserComponent* c : userComponents) {
		if (c->isActive() && !c->isSleeping() && !c->hasStarted())
			c->start();
	}
}


void Scene::preUpdate(float deltaTime)
{
	//Preupdate components
	for (UserComponent* c : userComponents) {
		if (c->isActive() && !c->isSleeping() && c->hasStarted())
			c->preUpdate(deltaTime);
	}

}


void Scene::update(float deltaTime)
{
	// update components
	for (UserComponent* c : userComponents) {
		if (c->isActive() && !c->isSleeping() && c->hasStarted())
			c->update(deltaTime);
	}
}


void Scene::postUpdate(float deltaTime)
{
	// postUpdate compoenent
	for (UserComponent* c : userComponents) {
		if (c->isActive() && !c->isSleeping() && c->hasStarted())
			c->postUpdate(deltaTime);
	}
}


bool Scene::addGameObject(GameObject* gameObject)
{
	// TODO: Antes deberia de mirar si el objeto ya existe, o si hay alguno con el mismo nombre o tag
	/* Hacer aqui */
	if (getGameObjectWithName(gameObject->getName()) != nullptr) {
		printf("SCENE: Trying to add gameobject with name %s that already exists in scene %s.\n", gameObject->getName().c_str(), name.c_str());
		return false;
	}

	sceneObjects.push_back(gameObject);
	return true;
}


GameObject* Scene::getGameObjectWithName(const std::string& name)
{
	for (auto g : sceneObjects) {
		if (g->getName() == name)
			return g;
	}
	return nullptr;
}


std::vector<GameObject*> Scene::getGameObjectsWithTag(const std::string& tag)
{
	std::vector<GameObject*> result;
	
	for (auto g : sceneObjects) {
		if (g->getTag() == tag) {
			result.push_back(g);
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

void Scene::destroyPendingGameObjects()
{
	if (!destroyQueue.size())
		return;

	for (GameObject* g : destroyQueue) {
		// Sacarlo de almacenamiento
		std::vector<GameObject*>::iterator it = std::find(sceneObjects.begin(), sceneObjects.end(), g);
		sceneObjects.erase(it);
		delete g;
		g = nullptr;
	}
}

void Scene::destroyGameObject(GameObject* gameObject)
{
	destroyQueue.push_back(gameObject);
}

void Scene::updateAllAnimations(float deltaTime)
{
	for (auto anim : sceneManager->getAnimationStates()) 
	{
		anim.second->addTime(deltaTime);
	}
}

void Scene::addUserComponent(UserComponent* component)
{
	userComponents.push_back(component);
}


const std::string& Scene::getName()
{
	return name;
}

Ogre::SceneManager* Scene::getSceneManager() const
{
	return sceneManager;
}

Ogre::Entity* Scene::createEntity(const std::string& name)
{
	return sceneManager->createEntity(name);
}