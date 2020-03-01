#include "RenderComponent.h"
#include "GameObject.h"

RenderComponent::RenderComponent(GameObject* gameObject) :  GaiaComponent(gameObject)
{
	node = RenderSystem::GetInstance()->getSceneManager()->createSceneNode();
}

RenderComponent::~RenderComponent() {}

Ogre::SceneNode* RenderComponent::getNode()
{
	return node;
}

void RenderComponent::createEntity(std::string id, std::string mesh)
{
	if (entities.find(id) == entities.end())
		entities[id] = RenderSystem::GetInstance()->createEntity(mesh);
}

void RenderComponent::setMaterial(std::string id, std::string material)
{
	entities[id]->setMaterialName(material);
}

void RenderComponent::setVisible(bool visible)
{
	node->setVisible(visible);
}

bool RenderComponent::isVisible()
{
	return visible;
}

void RenderComponent::addChildNode(std::string tag)
{
	/*RenderComponent* aux = findGameObjectWithTag(tag)->getComponent<RenderComponent>();

	if (aux != nullptr)
		node->addChild(aux->getNode());*/
}