#pragma once
#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <OgreSceneNode.h>
#include <string>
#include <map>

#include "Transform.h"

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

	const std::string getName();
	const std::string getTag();

	Scene* getScene();

private: 
    std::string name;
    std::string tag;

    Scene* myScene;
    std::map<std::string, Component*> components;
};

#endif