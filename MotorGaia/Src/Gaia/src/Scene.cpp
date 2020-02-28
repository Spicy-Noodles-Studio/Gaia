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

	sceneObjects.push_back(gameObject);
	return true;
}

bool Scene::delGameObjectWithName(std::string name)
{
	return false;
}

bool Scene::delGameObjectWithTag(std::string tag)
{
	return false;
}

GameObject* Scene::findGameObjectWithName(std::string name)
{
	return nullptr;
}

GameObject* Scene::findGameObjectWithTag(std::string tag)
{
	return nullptr;
}

inline std::string Scene::getSceneName()
{
	return name;
}
