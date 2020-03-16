#pragma once
#ifndef COMPONENT_MANAGER_H
#define COMPONENT_MANAGER_H

#include <Singleton.h>
#include <map>
#include <functional>
#include "Component.h"
#include "DebugUtils.h"

typedef std::function<Component* (GameObject*)> ComponentFactory;

class GAIA_API ComponentManager : public Singleton<ComponentManager>
{
public:
	ComponentManager();
	~ComponentManager();

	void init();
	void close();
	
	const ComponentFactory& getComponentFactory(const std::string& nameID);

	template<typename T>
	void registerComponent(const std::string& nameID);

	template<typename T>
	const std::string& getID();
	
private:
	std::map<std::string, ComponentFactory> factories;
	std::map<std::string, std::string> ids;
};

#endif

template<typename T>
void ComponentManager::registerComponent(const std::string& nameID)
{
	if (factories.find(nameID) != factories.end())
	{
		LOG("COMPONENT MANAGER: trying to register a ComponentFactory ID: %s, there is already an existing one.\n", nameID.c_str());
		return;
	}
	ids[typeid(T).name()] = nameID;
	factories[nameID] = [](GameObject* gameObject) { return new T(gameObject); };
}

template<typename T>
inline const std::string& ComponentManager::getID()
{
	return ids[typeid(T).name()];
}