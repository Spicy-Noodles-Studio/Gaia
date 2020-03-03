#include "Scene.h"

Scene::Scene(std::string sceneName) : name(sceneName)
{
}

Scene::~Scene()
{
}

bool Scene::addGameObject(GameObject* gameObject)
{
	// Antes deberia de mirar si el objeto ya existe, o si hay alguno con el mismo nombre o tag
	/* Hacer aqui */
	if (findGameObjectWithName(gameObject->getName()) != nullptr) {
		printf("SCENE: Trying to add gameobject with name %s that already exists in scene %s.\n", gameObject->getName().c_str(), name.c_str());
		return false;
	}

	sceneObjects.push_back(gameObject);
	return true;
}

bool Scene::delGameObjectWithName(const std::string& name)
{
	return false;
}

bool Scene::delGameObjectWithTag(const std::string& tag)
{
	return false;
}

GameObject* Scene::findGameObjectWithName(const std::string& name)
{
	return nullptr;
}

GameObject* Scene::findGameObjectWithTag(const std::string& tag)
{
	return nullptr;
}

inline std::string Scene::getSceneName()
{
	return name;
}
