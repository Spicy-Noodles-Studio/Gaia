#include "MeshRenderer.h"
#include "GameObject.h"

MeshRenderer::MeshRenderer(GameObject* gameObject) :  GaiaComponent(gameObject)
{

}

MeshRenderer::~MeshRenderer()
{

}

Ogre::Entity* MeshRenderer::getEntity(std::string entity)
{
	return entities[entity];
}

void MeshRenderer::createEntity(std::string id, std::string mesh)
{
	if (entities.find(id) == entities.end())
		entities[id] = RenderSystem::GetInstance()->createEntity(mesh);
}

void MeshRenderer::setMaterial(std::string id, std::string material)
{
	if (entities.find(id) != entities.end())
		entities[id]->setMaterialName(material);
}

void MeshRenderer::attachEntityToNode(std::string entity)
{
	gameObject->node->attachObject(entities[entity]);
}

void MeshRenderer::attachEntityToBone(std::string owner, std::string bone, std::string entity)
{
	entities[owner]->attachObjectToBone(bone, entities[entity]);
}

void MeshRenderer::setVisible(bool visible)
{
	gameObject->node->setVisible(visible);
}

bool MeshRenderer::isVisible()
{
	return visible;
}