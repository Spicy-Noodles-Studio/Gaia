#include "GameObject.h"
#include "RenderSystem.h"

#include <typeinfo>
#include <algorithm>

//#include "ComponentManager.h"

GameObject::GameObject(std::string name, std::string tag, Scene* scene) : name(name), tag(tag), myScene(scene)
{
	node = RenderSystem::GetInstance()->getSceneManager()->getRootSceneNode()->createChildSceneNode();
}

GameObject::~GameObject()
{

}

void GameObject::addChildNode(GameObject* child)
{
	if (node != nullptr)
		node->addChild(child->node);
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

void GameObject::onCollision(GameObject* other)
{
    //printf("Collision between %s & %s.\n", name.c_str(), other->getName().c_str());
}

void GameObject::onTrigger(GameObject* other)
{
    //printf("%s entered the trigger %s.\n", name.c_str(), other->getName().c_str());
}

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

    if(components.find(key) == components.end())
        return nullptr;

    return (T*)components[key];
}