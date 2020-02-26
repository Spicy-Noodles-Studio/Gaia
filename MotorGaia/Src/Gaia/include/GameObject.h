#pragma once

#include <string>
#include <map>

#include "Component.h"

class Scene;

class GameObject
{
public:
    GameObject(std::string name, std::string tag, Scene* scene);
    ~GameObject();

    template<typename T>
    T* addComponent();

    template<typename T>
    bool delComponent();

    template<typename T>
    T* getComponent();

private:
    inline Scene* getScene();

private: 
    std::string name;
    std::string tag;

    Scene* myScene;
    std::map<std::string, Component*> components;

};