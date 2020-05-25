#include "UserComponent.h"
#include "GameObject.h"
#include "Scene.h"
#include "ResourcesManager.h"
#include "ComponentManager.h"

UserComponent::UserComponent(GameObject* gameObject) : Component(gameObject), started(false), sleeping(false)
{
	checkNullAndBreak(gameObject);
	checkNullAndBreak(gameObject->myScene);

	gameObject->addUserComponent(this);
	gameObject->myScene->addUserComponent(this);
}


UserComponent::~UserComponent()
{

}


void UserComponent::awake()
{

}


void UserComponent::start()
{

}


void UserComponent::preUpdate(float deltaTime)
{

}


void UserComponent::update(float deltaTime)
{

}


void UserComponent::postUpdate(float deltaTime)
{

}

void UserComponent::fixedUpdate(float deltaTime)
{

}


void UserComponent::onCollisionEnter(GameObject* other)
{

}


void UserComponent::onCollisionStay(GameObject* other)
{

}


void UserComponent::onCollisionExit(GameObject* other)
{

}

GameObject* UserComponent::instantiate(const std::string& blueprintName, const Vector3& position)
{
	// check if blueprint name is valid
	const GameObjectData* data = ResourcesManager::getBlueprint(blueprintName);
	if (data == nullptr) return nullptr;

	GameObjectData d = GameObjectData(*data);
	// create object and add to pending
	GameObject* instance = instantiate(&d);

	if (instance == nullptr) return nullptr;

	if (instance->transform != nullptr)
		instance->transform->setPosition(position);

	return instance;
}

GameObject* UserComponent::instantiate(const GameObjectData* data)
{
	checkNullAndBreak(data, nullptr);
	checkNullAndBreak(gameObject, nullptr);
	checkNullAndBreak(gameObject->getScene(), nullptr);

	GameObject* instance;
	if (data->blueprintRef != nullptr)
		instance = instantiate(data->blueprintRef->name);
	else
		instance = new GameObject(data->getName(), data->getTag(), gameObject->getScene());

	checkNullAndBreak(instance, nullptr);
	gameObject->getScene()->instantiate(instance);

	// Component
	for (auto compData : data->getComponentData()) {
		ComponentData* cData = compData;
		if (cData == nullptr) continue;

		ComponentManager* componentManager = ComponentManager::GetInstance();
		if (componentManager == nullptr) {
			LOG_ERROR("USER COMPONENT", "Error ocurred while instantiating a GameObject named \"%s\"", gameObject->getName().c_str());
			delete instance;
			return nullptr;
		}
		auto constructor = componentManager->getComponentFactory(cData->getName());
		if (constructor != nullptr)
		{
			Component* comp = (*constructor)(instance);
			if (comp == nullptr) continue;

			comp->handleData(cData);
			if (!instance->addComponent(cData->getName(), comp))
				delete comp;
		}
	}
	// For each child, create the child
	for (auto childData : data->getChildrenData()) {
		GameObject* child = instantiate(childData);
		if (child == nullptr) continue;

		instance->addChild(child);
	}

	return instance;
}

void UserComponent::destroy(GameObject* gameObject)
{
	checkNullAndBreak(gameObject);
	checkNullAndBreak(gameObject->myScene);

	gameObject->myScene->destroyGameObject(gameObject);
}

GameObject* UserComponent::findGameObjectWithName(const std::string& name)
{
	checkNullAndBreak(gameObject, nullptr);
	checkNullAndBreak(gameObject->myScene, nullptr);

	return gameObject->myScene->getGameObjectWithName(name);
}

std::vector<GameObject*> UserComponent::findGameObjectsWithTag(const std::string& tag)
{
	checkNullAndBreak(gameObject, std::vector<GameObject*>());
	checkNullAndBreak(gameObject->myScene, std::vector<GameObject*>());

	return gameObject->myScene->getGameObjectsWithTag(tag);
}

void UserComponent::dontDestroyOnLoad(GameObject* gameObject)
{
	checkNullAndBreak(gameObject);
	checkNullAndBreak(gameObject->myScene);

	gameObject->myScene->dontDestroyOnLoad(gameObject);
}

bool UserComponent::hasStarted()
{
	return started;
}

bool UserComponent::isSleeping()
{
	return sleeping;
}

void UserComponent::onTriggerEnter(GameObject* other)
{

}

void UserComponent::onTriggerStay(GameObject* other)
{

}

void UserComponent::onTriggerExit(GameObject* other)
{

}

void UserComponent::onObjectEnter(GameObject* other)
{
}

void UserComponent::onObjectStay(GameObject* other)
{
}

void UserComponent::onObjectExit(GameObject* other)
{
}
