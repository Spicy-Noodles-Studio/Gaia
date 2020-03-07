#pragma once
#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <OgreSceneNode.h>
#include <string>
#include <map>

#include "ComponentManager.h"
#include "UserComponent.h"
#include "Transform.h"

class Scene;

class GameObject {
    friend class SceneManager;
    friend class UserComponent;
public:
    GameObject(const std::string& name, const std::string& tag, Scene* scene);
    ~GameObject();

	Ogre::SceneNode* node = nullptr;
	Transform* transform = nullptr;

    template<typename T>
    T* addComponent();

    template<typename T>
    bool delComponent();

    template<typename T>
    T* getComponent();
    
	void addChildNode(GameObject* child);
    const std::string& getName() const;
    const std::string& getTag() const;
    Scene* getScene() const;

private:
    void addUserComponent(UserComponent* component);
    bool addComponent(const std::string& name, Component* component);


private: 
    const std::string name;
    const std::string tag;

    Scene* myScene;
    std::map<std::string, Component*> components;
    std::vector<UserComponent*> userComponents;
};


template<typename T>
T* GameObject::addComponent() {
    const std::string key = Component::nameID<T>.id;
    if (components.find(key) != components.end()) {
        printf("GAMEOBJECT: Component %s already exists in %s GameObject\n", key.c_str(), name.c_str());
        return (T*)components[key]; // Return the exiting one.
    }
    auto constructor = ComponentManager::getComponentFactory(Component::nameID<T>.id);

    if (constructor == nullptr) {
        printf("GAMEOBJECT: Component %s not attached to %s GameObject. Constructor not found\n", key.c_str(), name.c_str());
        return nullptr;
    }

    components[key] = constructor(this);
    return (T*)components[key];
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
    const std::string key = Component::nameID<T>.id;
    if (components.find(key) == components.end())
        return nullptr;

    return (T*)components[key];
}

#endif