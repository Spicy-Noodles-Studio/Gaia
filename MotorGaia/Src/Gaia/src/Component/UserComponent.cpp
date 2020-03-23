#include "UserComponent.h"
#include "GameObject.h"
#include "Scene.h"
#include "ResourcesManager.h"
#include "ComponentManager.h"

UserComponent::UserComponent(GameObject* gameObject) : Component(gameObject), started(false), sleeping(false)
{
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
	// create object and add to pending
	GameObject* instance = instantiate(data);

	if (instance->transform != nullptr)
		instance->transform->setPosition(position);

	return instance;
}

GameObject* UserComponent::instantiate(const GameObjectData* data)
{
	GameObject* instance = new GameObject(data->getName(), data->getTag(), gameObject->getScene());
	// Component
	for (auto compData : data->getComponentData()) {
		ComponentData* cData = compData.second;
		auto constructor = ComponentManager::GetInstance()->getComponentFactory(cData->getName());
		if (constructor != nullptr)
		{
			Component* comp = constructor(instance);
			comp->handleData(cData);
			if (!instance->addComponent(cData->getName(), comp))
				delete comp;
		}
	}
	// For each child, create the child
	for (auto childData : data->getChildrenData()) {
		GameObject* child = instantiate(childData.second);
		instance->addChild(child);
	}

	gameObject->getScene()->instantiate(instance);

	return instance;
}

void UserComponent::destroy(GameObject* gameObject)
{
	gameObject->myScene->destroyGameObject(gameObject);
}

GameObject* UserComponent::findGameObjectWithName(const std::string& name)
{
	return gameObject->myScene->getGameObjectWithName(name);
}

std::vector<GameObject*> UserComponent::findGameObjectsWithTag(const std::string& tag)
{
	return gameObject->myScene->getGameObjectsWithTag(tag);
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
