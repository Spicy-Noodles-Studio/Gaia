#include "Component.h"

inline GameObject* Component::getOwner()
{
	return gameObject;
}

Component::Component(GameObject* gameObject) : gameObject(gameObject) {}