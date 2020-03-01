#include "SceneManager.h"
#include "ResourcesManager.h"
#include "ComponentManager.h"
#include "GameObject.h"
#include "SceneData.h"

SceneManager::SceneManager() : currentScene(nullptr)
{
}

SceneManager::~SceneManager()
{
}

bool SceneManager::createScene(std::string name) {

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
		scenes[name] = myScene;
		return created;
	}
	else
		return false;
}

bool SceneManager::exist(std::string name)
{
	return scenes.find(name) == scenes.end();
}


