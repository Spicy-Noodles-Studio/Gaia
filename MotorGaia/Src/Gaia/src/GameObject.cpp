#include "GameObject.h"

#include <typeinfo>
#include <algorithm>

//#include "ComponentManager.h"

GameObject::GameObject(std::string name, std::string tag, Scene* scene) : name(name), tag(tag), myScene(scene) {

}

GameObject::~GameObject() {
    
}

inline Scene* GameObject::getScene()
{
	return myScene;
}

template<typename T>
T* GameObject::addComponent() {
   std::string key = typeid(T).name();
    if(components.find(key) != components.end())
        return false;
    // Usar el gestor de componentes
    // y su mapa de constructoras
    /*auto constructor = ComponentManager::getConstructor(key);

    if(constructor == nullptr)
        printf("Error al añadir componente: %s, al GameObject con nombre: %s", key, name)

    components[key] = constructor();*/

    return true;
}

template<typename T>
bool GameObject::delComponent() {
    // Como el gestor de componentes los crea,
    // seria oportuno que los borrara.
}

template<typename T>
T* GameObject::getComponent() {
    std::string key = typeid(T).name();
    if(components.find(key) == components.end())
        return nullptr;

    return (T*)components[key];
}