#include "ComponentManager.h"

ComponentManager::ComponentManager()
{

}

ComponentManager::~ComponentManager()
{

}

void ComponentManager::init()
{

}

void ComponentManager::close()
{
	factories.clear();
	destroy();
}

ComponentFactory* ComponentManager::getComponentFactory(const std::string& nameID)
{
	if (factories.find(nameID) == factories.end())
	{
		LOG("COMPONENT MANAGER: trying to get a ComponentFactory ID: %s. Does not exist", nameID.c_str());
		return nullptr;
	}
	return &factories[nameID];
}