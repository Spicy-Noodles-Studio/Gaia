#pragma once

#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <string>
#include <map>

#include "Component.h"

class Scene;

class GameObject {
public:
    GameObject(std::string name, std::string tag, Scene* scene);
    ~GameObject();

    template<typename T>
    T* addComponent();

    template<typename T>
    bool delComponent();

    template<typename T>
    T* getComponent();

    bool addComponent(const std::string& componentName, Component* component);
    
    const std::string& getName() const;
    const std::string& getTag() const ;

private:
    inline Scene* getScene();

public:

private: 
    std::string name;
    std::string tag;

    Scene* myScene;
    std::map<std::string, Component*> components;

};

#endif