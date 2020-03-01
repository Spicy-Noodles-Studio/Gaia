#include "ComponentManager.h"

std::map<std::string, ComponentFactory> ComponentManager::factories;

ComponentManager::ComponentManager()
{
}

ComponentManager::~ComponentManager()
{
}

const ComponentFactory ComponentManager::getComponentFactory(const std::string& nameID)
{
	if (factories.find(nameID) == factories.end())
	{
		printf("COMPONENT MANAGER: trying to get a ComponentFactory ID: %s. Does not exist.\n", nameID.c_str());
		return nullptr;
	}
	return factories[nameID];
}
