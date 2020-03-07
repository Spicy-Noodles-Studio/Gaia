#pragma once
#ifndef COMPONENT_H
#define COMPONENT_H

#include <string>

class GameObject;
class ComponentData;

template<typename T>
struct ComponentID
{
	std::string id;
};

class Component
{
friend class ComponentManager;
friend class GameObject;
public:
	Component(GameObject* gameObject);

	//Da valor a las variables del componente segun el ComponentData que reciba
	//Usando if else para elegir la propiedad que modifica if(ComponentData->name == name)
	//Usando sstream se da valor a la variable ->                sstream>>variable
	virtual void handleData(ComponentData* data);

	void setActive(bool active);
	bool isActive();

	template<typename T>
	static const std::string& getID();

public:
	GameObject* gameObject;

private:
	template<typename T>
	static ComponentID<T> nameID;

	bool active;
};

template<typename T>
ComponentID<T> Component::nameID = { "not assigned" };


template<typename T>
inline const std::string& Component::getID()
{
	return nameID<T>.id;
}

#endif
