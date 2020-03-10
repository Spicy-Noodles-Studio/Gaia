#include "ComponentManager.h"

#include "Transform.h"
#include "Camera.h"
#include "Light.h"
#include "MeshRenderer.h"
#include "UILayout.h"

ComponentManager::ComponentManager()
{

}


ComponentManager::~ComponentManager()
{

}


void ComponentManager::init()
{
	// Register Gaia base components 
	registerComponent<Transform>("Transform");
	registerComponent<Camera>("Camera");
	registerComponent<Light>("Light");
	registerComponent<MeshRenderer>("MeshRenderer");
	registerComponent<UILayout>("UILayout");
}

void ComponentManager::close()
{
	factories.clear();
	destroy();
}

const ComponentFactory& ComponentManager::getComponentFactory(const std::string& nameID)
{
	if (factories.find(nameID) == factories.end())
	{
		printf("COMPONENT MANAGER: trying to get a ComponentFactory ID: %s. Does not exist.\n", nameID.c_str());
		return nullptr;
	}
	return factories[nameID];
}
