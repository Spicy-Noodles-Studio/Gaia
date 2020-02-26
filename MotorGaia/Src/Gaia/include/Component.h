#pragma once
#include <string>

class GameObject;

class Component
{
public: 
    // Devuelve el componente del tipo indicado suponiendo que existe en el mismo owner,
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

private:
    GameObject* gameObject;

};