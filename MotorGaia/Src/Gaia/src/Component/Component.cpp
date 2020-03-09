#include "Component.h"


Component::Component(GameObject* gameObject) : gameObject(gameObject), active(true) 
{

}

Component::~Component()
{
}

void Component::setActive(bool active)
{
	this->active = active;
}

bool Component::isActive()
{
	return active;
}


void Component::handleData(ComponentData* data)
{

}
