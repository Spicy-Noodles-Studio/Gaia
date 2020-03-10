#pragma once
#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <OgreSceneNode.h>
#include <string>
#include <map>

#include "Transform.h"
#include "UserComponent.h"

class Scene;

class GameObject
{
public:
    GameObject(std::string name, std::string tag, Scene* scene);
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

    bool addComponent(const std::string& componentName, Component* component);
    
    const std::string& getName() const;
    const std::string& getTag() const ;
	const Scene* getScene() const;

    void onCollisionEnter(GameObject* other);
    void onTriggerEnter(GameObject* other);

    void onCollisionStay(GameObject* other);
    void onTriggerStay(GameObject* other);

    void onCollisionExit(GameObject* other);
    void onTriggerExit(GameObject* other);

private: 
    std::string name;
    std::string tag;

    Scene* myScene;
    std::map<std::string, Component*> components;
    std::vector<UserComponent*> userComponents;
};

#endif