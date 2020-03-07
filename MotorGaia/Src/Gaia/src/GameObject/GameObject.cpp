#include "GameObject.h"
#include "Scene.h"


GameObject::GameObject(const std::string& name, const std::string& tag, Scene* scene) : name(name), tag(tag), myScene(scene)
{
	node = scene->getSceneManager()->getRootSceneNode()->createChildSceneNode();
}

GameObject::~GameObject()
{
    node->detachAllObjects();

    for (auto c : components) {
        delete c.second;
        c.second = nullptr;
    }
    components.clear();
    userComponents.clear();
}

void GameObject::addChildNode(GameObject* child)
{
	if (node != nullptr)
		node->addChild(child->node);
}

const std::string& GameObject::getName() const
{
    return name;
}

const std::string& GameObject::getTag() const 
{
    return tag;
}

Scene* GameObject::getScene() const
{
	return myScene;
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

void GameObject::addUserComponent(UserComponent* component)
{
    userComponents.push_back(component);
}
