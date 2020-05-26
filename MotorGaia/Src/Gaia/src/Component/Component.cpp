#include "Component.h"


Component::Component(GameObject* gameObject) : gameObject(gameObject), active(true) 
{

}

Component::~Component()
{
	gameObject = nullptr;
}

void Component::setActive(bool active)
{
	this->active = active;
}

bool Component::isActive() const
{
	return active;
}


void Component::handleData(ComponentData* data)
{

}
