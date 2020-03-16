#include "Scene.h"
#include <OgreRoot.h>
#include "PhysicsSystem.h"
#include "GameObject.h"
#include "Camera.h"
#include "DebugDrawer.h"

Scene::Scene(const std::string& sceneName, Ogre::Root* root) : name(sceneName), root(root), sceneManager(root->createSceneManager()), mainCamera(nullptr)
{
	debugDrawer = new DebugDrawer(sceneManager);
	PhysicsSystem::GetInstance()->setDebugDrawer(debugDrawer);
}

Scene::~Scene()
{
	for (GameObject* gameObject : sceneObjects) {
		delete gameObject;
		gameObject = nullptr;
	}

	for (GameObject* gameObject : instantiateQueue) {
		delete gameObject;
		gameObject = nullptr;
	}

	sceneObjects.clear();
	destroyQueue.clear();
	instantiateQueue.clear();

	sceneManager->clearScene();
	root->destroySceneManager(sceneManager);

	mainCamera = nullptr;

	animationSets.clear();

	delete debugDrawer;
}

void Scene::awake()
{
	// awake components
	for (UserComponent* c : userComponents) {
		if (c->isActive() && c->isSleeping()) {
			c->awake();
			c->sleeping = false;
		}
	}
}

void Scene::start()
{
	// start components
	for (UserComponent* c : userComponents) {
		if (c->isActive() && !c->isSleeping() && !c->hasStarted()) {
			c->start();
			c->started = true;
		}
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
	// TODO: Antes deberia de mirar si el objeto ya existe, o si hay alguno con el mismo nombre o tag
	/* Hacer aqui */
	std::string objectName = gameObject->getName();
	if (repeatedNames.find(objectName) != repeatedNames.end()) {
		objectName += ("(" + std::to_string(++repeatedNames[objectName]) + ")");
		LOG("SCENE: Trying to add gameobject with name %s that already exists in scene %s\n", gameObject->getName().c_str(), name.c_str());
		LOG("SCENE: Adding gameobject with name %s\n", objectName.c_str());
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

	for (GameObject* g : destroyQueue) {
		// Sacarlo de almacenamiento
		std::vector<GameObject*>::iterator it = std::find(sceneObjects.begin(), sceneObjects.end(), g);
		sceneObjects.erase(it);
		delete g;
		g = nullptr;
	}
	destroyQueue.clear();
}

void Scene::destroyGameObject(GameObject* gameObject)
{
	destroyQueue.push_back(gameObject);
}

void Scene::instantiate(GameObject* gameObject)
{
	gameObject->node->setVisible(false);
	gameObject->setActive(false);
	instantiateQueue.push_back(gameObject);
}

void Scene::instantiatePendingGameObjects()
{
	if (!instantiateQueue.size()) return;

	for (auto gameObject : instantiateQueue) {
		gameObject->node->setVisible(true);
		gameObject->setActive(true);
		addGameObject(gameObject);
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