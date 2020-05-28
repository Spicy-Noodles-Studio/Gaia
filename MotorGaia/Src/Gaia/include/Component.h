#pragma once
#ifndef COMPONENT_H
#define COMPONENT_H

#include <string>
#include <sstream>
#include "ComponentData.h"
#include "DebugUtils.h"
#include "Macros.h"
#include "ErrorManagement.h"

class GameObject;

class GAIA_API Component
{
friend class ComponentManager;
friend class GameObject;
friend class UserComponent;
friend class SceneManager;

public:
	Component(GameObject* gameObject);
	virtual ~Component();

	virtual void setActive(bool active);
	bool isActive() const;

protected:
	//Da valor a las variables del componente segun el ComponentData que reciba
	//Usando if else para elegir la propiedad que modifica if(ComponentData->name == name)
	//Usando sstream se da valor a la variable ->                sstream>>variable
	virtual void handleData(ComponentData* data);

public:
	GameObject* gameObject;

private:
	bool active;
};


#endif
