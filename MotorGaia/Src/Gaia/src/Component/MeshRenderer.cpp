#include "MeshRenderer.h"
#include "GameObject.h"
#include "Scene.h"
#include "ComponentData.h"
#include <sstream>

MeshRenderer::MeshRenderer(GameObject* gameObject) : GaiaComponent(gameObject)
{

}

MeshRenderer::~MeshRenderer()
{

}

void MeshRenderer::setMesh(std::string id, std::string mesh)
{
	if (entities.find(id) == entities.end())
	{
		// TODO: intentar quitar esta dependencis
		entities[id] = gameObject->getScene()->createEntity(mesh);
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
