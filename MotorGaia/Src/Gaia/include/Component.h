#pragma once
#ifndef COMPONENT_H
#define COMPONENT_H

#include <string>
#include "ComponentData.h"
#include "DebugUtils.h"

class GameObject;

class GAIA_API Component
{
friend class ComponentManager;
friend class GameObject;
public:

	Component(GameObject* gameObject);
	virtual ~Component();

	//Da valor a las variables del componente segun el ComponentData que reciba
	//Usando if else para elegir la propiedad que modifica if(ComponentData->name == name)
	//Usando sstream se da valor a la variable ->                sstream>>variable
	virtual void handleData(ComponentData* data);

	virtual void setActive(bool active);
	bool isActive();

public:
	GameObject* gameObject;

private:
	bool active;
};


#endif
