#include "UserComponent.h"
#include "GameObject.h"
#include "Scene.h"


UserComponent::UserComponent(GameObject* gameObject) : Component(gameObject), started(false), sleeping(false)
{
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


void UserComponent::destroy(GameObject* gameObject)
{
	gameObject->myScene->destroyGameObject(gameObject);
}


bool UserComponent::hasStarted()
{
	return started;
}


bool UserComponent::isSleeping()
{
	return sleeping;
}


void UserComponent::setSleeping(bool sleep)
{
	sleeping = sleep;
}
