#include "RenderComponent.h"

RenderComponent::RenderComponent(GameObject* gameObject) :  GaiaComponent(gameObject)
{
	node = RenderSystem::GetInstance()->getSceneManager()->createSceneNode();
	nodes["Root"] = node;
	nodesVisibility["Root"] = true;
}

RenderComponent::~RenderComponent()
{

}

void RenderComponent::createEntity(Ogre::Entity* entity, std::string mesh, std::string id)
{
	if (entities.find(id) == entities.end())
		entities[id] = RenderSystem::GetInstance()->createEntity(entity, mesh);
}

void RenderComponent::setMaterial(Ogre::Entity* entity, std::string material)
{
	RenderSystem::GetInstance()->setMaterial(entity, material);
}

void RenderComponent::createChildNode(Ogre::Entity* entity, std::string id)
{
	if (entities.find(id) == entities.end())
	{
		nodes[id] = RenderSystem::GetInstance()->createChildNode(node, entity, id);
		nodesVisibility[id] = true;
	}
}

void RenderComponent::setPosition(Ogre::SceneNode* node, Ogre::Vector3 position)
{
	node->setPosition(position);
}

void RenderComponent::setScale(Ogre::SceneNode* node, Ogre::Vector3 scale)
{
	node->setScale(scale);
}

void RenderComponent::setRotation(Ogre::SceneNode* node, Axis axis, float degrees)
{
	switch (axis)
	{
	case X:
		node->pitch(Ogre::Radian(Ogre::Degree(degrees)));
		break;
	case Y:
		node->yaw(Ogre::Radian(Ogre::Degree(degrees)));
		break;
	case Z:
		node->roll(Ogre::Radian(Ogre::Degree(degrees)));
		break;
	default:
		break;
	}
}

void RenderComponent::setVisible(Ogre::SceneNode* node, bool visible)
{
	node->setVisible(visible);
}

bool RenderComponent::isVisible(std::string id)
{
	if (entities.find(id) != entities.end())
		return nodesVisibility[id];
}