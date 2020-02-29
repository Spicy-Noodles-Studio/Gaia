#pragma once

#ifndef COMPONENT_H
#define COMPONENT_H

#include <string>

class GameObject;

template<typename T>
struct ComponentID 
{
    std::string id;
};

class Component 
{
public: 
    friend class ComponentManager;
    // Devuelve el componente de tipo indicado suponiendo que existe en el mismo owner,
    // si no, devuelve un nullptr  
    template<typename T>
    T* getComponent();

    // Busca a partir de la referencia de Scene desde el owner un objeto en la misma escena
    // con el nombre indicado
    template<typename T>
    GameObject* findGameObjectWithName(std::string name);

    // Busca a partir de la referencia de Scene desde el owner un objeto en la misma escena
    // con el tag indicado
    template<typename T>
    GameObject* findGameObjectWithTag(std::string tag);

protected:
    Component(GameObject* gameObject);

public:
    template<typename T>
    static const std::string& getID();

private:
    GameObject* gameObject;
    
    template<typename T>
    static ComponentID<T> nameID;

};


template<typename T>
ComponentID<T> Component::nameID = { "not assigned" };

template<typename T>
inline const std::string& Component::getID()
{
    return nameID<T>.id;
}

#endif
