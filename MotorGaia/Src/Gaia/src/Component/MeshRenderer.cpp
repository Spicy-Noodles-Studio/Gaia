#include "MeshRenderer.h"
#include "GameObject.h"
#include "Scene.h"
#include "ComponentData.h"
#include <sstream>

MeshRenderer::MeshRenderer(GameObject* gameObject) : GaiaComponent(gameObject), visible(true)
{

}

MeshRenderer::~MeshRenderer()
{
	for (auto entity : entities) 
		gameObject->getScene()->getSceneManager()->destroyEntity(entity.second);
	
	entities.clear();
}

void MeshRenderer::setMesh(const std::string& id, const std::string& mesh)
{
	if (entities.find(id) == entities.end())
	{
		entities[id] = gameObject->getScene()->createEntity(mesh);
		gameObject->node->attachObject(entities[id]);
	}
}

void MeshRenderer::setMaterial(const std::string& id, const std::string& material)
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

void MeshRenderer::handleData(ComponentData* data)
{
	for (auto prop : data->getProperties()) {
		std::stringstream ss(prop.second);

		if (prop.first == "mesh") {
			std::string id, name; ss >> id >> name;
			setMesh(id, name);
		}
		else if (prop.first == "material") {
			std::string id, name; ss >> id >> name;
			setMaterial(id, name);
		}
	}
}
