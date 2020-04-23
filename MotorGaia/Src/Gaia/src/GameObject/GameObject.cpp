#include "GameObject.h"
#include "Scene.h"
#include <OgreSceneNode.h>
#include <OgreSceneManager.h>
#include "SceneManager.h"


GameObject::GameObject(const std::string& name, const std::string& tag, Scene* scene) : name(name), tag(tag), active(true), myScene(scene)
{
	node = scene->getSceneManager()->getRootSceneNode()->createChildSceneNode(scene->sceneManager->getNextNodeID());
}

GameObject::~GameObject()
{
	for (auto c : components) {
		delete c.second;
		c.second = nullptr;
	}
	components.clear();
	userComponents.clear();

	if (parent != nullptr)
		parent->removeChild(this);

	for (GameObject* child : children) {
		removeChild(child);
	}
}

GameObject* GameObject::getParent() const
{
	return parent;
}

std::vector<GameObject*>& GameObject::getChildren()
{
	return children;
}

GameObject* GameObject::findChildWithName(const std::string& name)
{
	for (GameObject* child : children) {
		if (child->getName() == name)
			return child;
	}
	return nullptr;
}

std::vector<GameObject*> GameObject::findChildrenWithTag(const std::string& tag)
{
	std::vector<GameObject*> childrenFound;
	for (GameObject* child : children) {
		if (child->getTag() == tag)
			childrenFound.push_back(child);
	}
	return childrenFound;
}

void GameObject::addChild(GameObject* child)
{
	if (child == nullptr) return;

	// Check if it has parent and if son, remove it
	if (child->getParent() != nullptr)
		child->getParent()->removeChild(child);

	children.push_back(child);
	child->parent = this;

	// SceneNode stuff
	child->node->getParent()->removeChild(child->node);
	node->addChild(child->node);
}

void GameObject::removeChild(GameObject* child)
{
	if (child == nullptr) return;

	// Check if given child is in my children vector
	if (children.size() == 0) return;
	auto it = std::find(children.begin(), children.end(), child);
	if (it == children.end()) return;
	// Remove from children vector
	children.erase(it);
	child->parent = nullptr; // Dereference parent

	// SceneNode stuff
	node->removeChild(child->node);
	node->getCreator()->getRootSceneNode()->addChild(child->node);
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

void GameObject::onCollisionEnter(GameObject* other)
{
	for (UserComponent* c : userComponents)
		if (c->isActive())
			c->onCollisionEnter(other);
}

void GameObject::onTriggerEnter(GameObject* other)
{
	for (UserComponent* c : userComponents)
		if (c->isActive())
			c->onTriggerEnter(other);
}

void GameObject::onObjectEnter(GameObject* other)
{
	for (UserComponent* c : userComponents)
		if (c->isActive())
			c->onObjectEnter(other);
}

void GameObject::onCollisionStay(GameObject* other)
{
	for (UserComponent* c : userComponents)
		if (c->isActive())
			c->onCollisionStay(other);
}

void GameObject::onTriggerStay(GameObject* other)
{
	for (UserComponent* c : userComponents)
		if (c->isActive())
			c->onTriggerStay(other);
}

void GameObject::onObjectStay(GameObject* other)
{
	for (UserComponent* c : userComponents)
		if (c->isActive())
			c->onObjectStay(other);
}

void GameObject::onCollisionExit(GameObject* other)
{
	for (UserComponent* c : userComponents)
		if (c->isActive())
			c->onCollisionExit(other);
}

void GameObject::onTriggerExit(GameObject* other)
{
	for (UserComponent* c : userComponents)
		if (c->isActive())
			c->onTriggerExit(other);
}

void GameObject::onObjectExit(GameObject* other)
{
	for (UserComponent* c : userComponents)
		if (c->isActive())
			c->onObjectExit(other);
}

void GameObject::setActive(bool active)
{
	for (auto comp : components)
		comp.second->setActive(active);

	this->active = active;
}

bool GameObject::isActive() const
{
	return active;
}

bool GameObject::addComponent(const std::string& componentName, Component* component)
{
	if (components.find(componentName) != components.end()) {
		LOG("GAMEOBJECT: Trying to add component with name %s that already exists in gameobject %s.\n", componentName.c_str(), name.c_str());
		return false;
	}
	components[componentName] = component;
	return true;
}

void GameObject::addUserComponent(UserComponent* component)
{
	userComponents.push_back(component);
}