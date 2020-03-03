#pragma once

#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <string>
#include <map>

#include "Component.h"

class Scene;

class GameObject {
    friend class SceneManager;
    friend class UserComponent;
public:
    GameObject(const std::string& name, const std::string& tag, Scene* scene);
    ~GameObject();

    template<typename T>
    T* addComponent();

    template<typename T>
    bool delComponent();

    template<typename T>
    T* getComponent();
    
    const std::string& getName() const;
    const std::string& getTag() const;

private:
    bool addComponent(const std::string& componentName, Component* component);
    const Scene* getScene() const;

public:

private: 
    const std::string name;
    const std::string tag;

    Scene* myScene;

    std::map<std::string, Component*> components;
};

#endif