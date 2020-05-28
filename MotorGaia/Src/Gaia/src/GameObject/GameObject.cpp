#include "GameObject.h"
#include "SceneManager.h"


GameObject::GameObject(const std::string& name, const std::string& tag, Scene* scene) : name(name), tag(tag), active(true), myScene(scene),
																						transform (nullptr), node(nullptr), parent(nullptr)
{
	checkNullAndBreak(scene);
	checkNullAndBreak(scene->sceneManager);
	Ogre::SceneManager* sceneManager = scene->getSceneManager();
	checkNullAndBreak(sceneManager);
	Ogre::SceneNode* root = sceneManager->getRootSceneNode();
	checkNullAndBreak(root);

	node = root->createChildSceneNode(scene->sceneManager->getNextNodeID());
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
		if (child == nullptr) continue;

		if (child->getTag() == tag)
			childrenFound.push_back(child);
	}
	return childrenFound;
}

void GameObject::addChild(GameObject* child)
{
	if (child == nullptr) return;

	// Check if it has parent and if son, remove it
	GameObject* parent = child->getParent();
	if (parent != nullptr)
		parent->removeChild(child);

	children.push_back(child);
	child->parent = this;

	// SceneNode stuff
	checkNullAndBreak(node);
	checkNullAndBreak(child->node);
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
	checkNullAndBreak(node);
	checkNullAndBreak(child->node);
	node->removeChild(child->node);
	Ogre::SceneManager* sceneManager = node->getCreator();
	checkNullAndBreak(sceneManager);
	Ogre::SceneNode* root = sceneManager->getRootSceneNode();
	checkNullAndBreak(root);
	root->addChild(child->node);
}

const std::string& GameObject::getName() const
{
	return name;
}

void GameObject::setName(const std::string& name)
{
	this->name = name;
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
	std::vector<UserComponent*> components = userComponents;

	for (UserComponent* component : components) {
		checkNullAndBreak(component);

		if (component->isActive())
			component->onCollisionEnter(other);
	}
}

void GameObject::onTriggerEnter(GameObject* other)
{
	std::vector<UserComponent*> components = userComponents;

	for (UserComponent* component : components) {
		checkNullAndBreak(component);

		if (component->isActive())
			component->onTriggerEnter(other);
	}
}

void GameObject::onObjectEnter(GameObject* other)
{
	std::vector<UserComponent*> components = userComponents;

	for (UserComponent* component : components) {
		checkNullAndBreak(component);

		if (component->isActive())
			component->onObjectEnter(other);
	}
}

void GameObject::onCollisionStay(GameObject* other)
{
	std::vector<UserComponent*> components = userComponents;

	for (UserComponent* component : components) {
		checkNullAndBreak(component);

		if (component->isActive())
			component->onCollisionStay(other);
	}
}

void GameObject::onTriggerStay(GameObject* other)
{
	std::vector<UserComponent*> components = userComponents;

	for (UserComponent* component : components) {
		checkNullAndBreak(component);

		if (component->isActive())
			component->onTriggerStay(other);
	}
}

void GameObject::onObjectStay(GameObject* other)
{
	std::vector<UserComponent*> components = userComponents;

	for (UserComponent* component : components) {
		checkNullAndBreak(component);

		if (component->isActive())
			component->onObjectStay(other);
	}
}

void GameObject::onCollisionExit(GameObject* other)
{
	std::vector<UserComponent*> components = userComponents;

	for (UserComponent* component : components) {
		checkNullAndBreak(component);

		if (component->isActive())
			component->onCollisionExit(other);
	}
}

void GameObject::onTriggerExit(GameObject* other)
{
	std::vector<UserComponent*> components = userComponents;

	for (UserComponent* component : components) {
		checkNullAndBreak(component);

		if (component->isActive())
			component->onTriggerExit(other);
	}
}

void GameObject::onObjectExit(GameObject* other)
{
	std::vector<UserComponent*> components = userComponents;

	for (UserComponent* component : components) {
		checkNullAndBreak(component);

		if (component->isActive())
			component->onObjectExit(other);
	}
}

void GameObject::setActive(bool active)
{
	for (auto comp : components) {
		checkNull(comp.second);
		comp.second->setActive(active);
	}

	this->active = active;
}

bool GameObject::isActive() const
{
	return active;
}

bool GameObject::addComponent(const std::string& componentName, Component* component)
{
	checkNullAndBreak(component, false);

	if (components.find(componentName) != components.end()) {
		LOG("GAMEOBJECT: Trying to add component with name %s that already exists in gameobject %s.\n", componentName.c_str(), name.c_str());
		return false;
	}
	components[componentName] = component;
	return true;
}

void GameObject::addUserComponent(UserComponent* component)
{
	checkNullAndBreak(component);
	userComponents.push_back(component);
}