#include "MeshRenderer.h"
#include "GameObject.h"

MeshRenderer::MeshRenderer(GameObject* gameObject) :  GaiaComponent(gameObject)
{

}

MeshRenderer::~MeshRenderer()
{

}

void MeshRenderer::createEntity(std::string id, std::string mesh)
{
	if (entities.find(id) == entities.end())
	{
		entities[id] = RenderSystem::GetInstance()->createEntity(mesh);
		gameObject->node->attachObject(entities[id]);
	}
}

void MeshRenderer::setMaterial(std::string id, std::string material)
{
	if (entities.find(id) != entities.end())
		entities[id]->setMaterialName(material);
}

void MeshRenderer::setVisible(bool visible)
{
	gameObject->node->setVisible(visible);
}

bool MeshRenderer::isVisible()
{
	return visible;
}