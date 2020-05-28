#pragma once
#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <string>
#include <map>
#include <vector>

#include "ComponentManager.h"
#include "Transform.h"

namespace Ogre {
    class SceneNode;
}

class Scene;
class UserComponent;

class GAIA_API GameObject {
    friend class Scene;
    friend class SceneManager;
    friend class UserComponent;
    friend class PhysicsSystem;
public:
    GameObject(const std::string& name, const std::string& tag, Scene* scene);
    ~GameObject();

    template<typename T>
    T* addComponent();

    template<typename T>
    bool deleteComponent();

    template<typename T>
    T* getComponent();

    /* Get parent of current gameObject, return nullptr if has no parent */
    GameObject* getParent() const;

    /* Get all children */
    std::vector<GameObject*>& getChildren();

    /* Find child with given name */
    GameObject* findChildWithName(const std::string& name);
    
    /* Find children with a given tag */
    std::vector<GameObject*> findChildrenWithTag(const std::string& tag);

    /* Add children to current parent */
    void addChild(GameObject* child);

    /* Remove child from parent */
    void removeChild(GameObject* child);
    
    const std::string& getName() const;
    void setName(const std::string& name);
    const std::string& getTag() const;
    Scene* getScene() const;

    void setActive(bool active);
    bool isActive() const;

private:
	void onCollisionEnter(GameObject* other);
	void onTriggerEnter(GameObject* other);
	void onObjectEnter(GameObject* other);

	void onCollisionStay(GameObject* other);
	void onTriggerStay(GameObject* other);
	void onObjectStay(GameObject* other);

	void onCollisionExit(GameObject* other);
	void onTriggerExit(GameObject* other);
	void onObjectExit(GameObject* other);

private:
    void addUserComponent(UserComponent* component);
    bool addComponent(const std::string& name, Component* component);

public:
	Ogre::SceneNode* node;
	Transform* transform;

private: 
    std::string name;
    std::string tag;
    bool active;

    GameObject* parent;
    std::vector<GameObject*> children;

    Scene* myScene;
    std::map<std::string, Component*> components;
    std::vector<UserComponent*> userComponents;
};

template<typename T>
T* GameObject::addComponent() {
    ComponentManager* componentManager = ComponentManager::GetInstance();
    checkNullAndBreak(componentManager);

    const std::string key = componentManager->getID<T>();
    if (components.find(key) != components.end()) {
        LOG_ERROR("GAMEOBJECT", "Component %s already exists in %s GameObject", key.c_str(), name.c_str());
        return (T*)components[key]; // Return the exiting one.
    }
    auto constructor = componentManager->getComponentFactory(key);

    if (constructor == nullptr) {
        LOG_ERROR("GAMEOBJECT", "Component %s not attached to %s GameObject. Constructor not found", key.c_str(), name.c_str());
        return nullptr;
    }

    components[key] = constructor(this);
    return (T*)components[key];
}

template<typename T>
bool GameObject::deleteComponent() {
    ComponentManager* componentManager = ComponentManager::GetInstance();
    checkNullAndBreak(componentManager);

    const std::string key = componentManager->getID<T>();
    if (components.find(key) != components.end()) {
        LOG_ERROR("GAMEOBJECT", "Cannot remove. Component %s does not exist in %s GameObject", key.c_str(), name.c_str());
        return false;
    }

    delete components[key];
    components.erase(key);

    return true;
}

template<typename T>
T* GameObject::getComponent() {
    ComponentManager* componentManager = ComponentManager::GetInstance();
    checkNullAndBreak(componentManager, nullptr);

    const std::string key = componentManager->getID<T>();
    if (components.find(key) == components.end())
        return nullptr;

    return (T*)components[key];
}

#endif