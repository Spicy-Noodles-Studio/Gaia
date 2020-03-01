#pragma once

#ifndef COMPONENT_MANAGER_H
#define COMPONENT_MANAGER_H

#include "Component.h"
#include <map>
#include <functional>

typedef std::function<Component* (GameObject*)> ComponentFactory;

class ComponentManager
{
public:
	ComponentManager();
	~ComponentManager();
	
	static const ComponentFactory getComponentFactory(const std::string& nameID);

	template<typename T>
	static void registerComponent(const std::string& nameID);
	
private:
	static std::map<std::string, ComponentFactory> factories;

};

template<typename T>
static void ComponentManager::registerComponent(const std::string& nameID)
{
	if (factories.find(nameID) != factories.end())
	{
		printf("COMPONENT MANAGER: trying to register a ComponentFactory ID: %s, there is already an existing one.\n", nameID.c_str());
		return;
	}
	Component::nameID<T>.id = nameID;
	factories[nameID] = [](GameObject* gameObject) { return new T(gameObject); };
}


#endif