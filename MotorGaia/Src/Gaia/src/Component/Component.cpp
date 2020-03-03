#include "Component.h"


Component::Component(GameObject* gameObject) : gameObject(gameObject), active(true) 
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
