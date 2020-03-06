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

    bool addComponent(const std::string& componentName, Component* component);
    
    const std::string& getName() const;
    const std::string& getTag() const ;
	const Scene* getScene() const;

private: 
    std::string name;
    std::string tag;

    Scene* myScene;
    std::map<std::string, Component*> components;
};

template<typename T>
T* GameObject::addComponent()
{
	std::string key = typeid(T).name();

	if (components.find(key) != components.end())
		return false;

	// Usar el gestor de componentes
	// y su mapa de constructoras
	/*auto constructor = ComponentManager::getConstructor(key);

	components[key] = constructor();*/

	return true;
}

template<typename T>
bool GameObject::delComponent()
{
	// Como el gestor de componentes los crea,
	// seria oportuno que los borrara.
}

template<typename T>
T* GameObject::getComponent()
{
	std::string key = typeid(T).name();

	if (components.find(key) == components.end())
		return nullptr;

	return (T*)components[key];
}

#endif