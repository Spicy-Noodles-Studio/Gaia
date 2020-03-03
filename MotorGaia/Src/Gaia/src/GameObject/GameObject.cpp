#include "GameObject.h"

#include <typeinfo>
#include <algorithm>

#include "ComponentManager.h"

GameObject::GameObject(const std::string& name, const std::string& tag, const Scene* scene) : name(name), tag(tag), myScene(scene) {

}


GameObject::~GameObject() {
    
}


template<typename T>
T* GameObject::addComponent() {
    const std::string key = Component::nameID<T>;
    if (components.find(key) != components.end()) {
        printf("GAMEOBJECT: Component %s already exists in %s GameObject\n", key.c_str(), name.c_str());
        return false;
    }
    auto constructor = ComponentManager::getComponentFactory(Component::nameID<T>);

    if (constructor == nullptr) {
        printf("GAMEOBJECT: Component %s not attached to %s GameObject. Constructor not found\n", key.c_str(), name.c_str());
        return false;
    }

    components[key] = constructor();
    return true;
}

template<typename T>
bool GameObject::delComponent() {
    const std::string key = Component::nameID<T>;
    if (components.find(key) != components.end()) {
        printf("GAMEOBJECT: Cannot remove. Component %s does not exist in %s GameObject\n", key.c_str(), name.c_str());
        return false;
    }

    delete components[key];
    components.erase(key);

    return true;
}

template<typename T>
T* GameObject::getComponent() {
    const std::string key = Component::nameID<T>;
    if (components.find(key) == components.end())
        return nullptr;

    return (T*)components[key];
}

bool GameObject::addComponent(const std::string& componentName, Component* component)
{
    if (components.find(componentName) != components.end()) {
        printf("GAMEOBJECT: Trying to add component with name %s that already exists in gameobject %s.\n", componentName.c_str(), name.c_str());
        return false;
    }
    components[componentName] = component;
    return true;
}

const std::string& GameObject::getName() const
{
    return name;
}

const std::string& GameObject::getTag() const 
{
    return tag;
}

const Scene* GameObject::getScene() const
{
	return myScene;
}
