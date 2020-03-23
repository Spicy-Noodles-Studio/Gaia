#include "ComponentManager.h"

#include "Transform.h"
#include "Camera.h"
#include "Light.h"
#include "MeshRenderer.h"
#include "Animator.h"
#include "RigidBody.h"
#include "UILayout.h"
#include "SoundEmitter.h"
#include "SoundListener.h"
#include "Reverb.h"
#include "ParticleEmitter.h"
#include "Strider.h"

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
	registerComponent<Animator>("Animator");
	registerComponent<RigidBody>("RigidBody");
	registerComponent<UILayout>("UILayout");
	registerComponent<SoundEmitter>("SoundEmitter");
	registerComponent<SoundListener>("SoundListener");
	registerComponent<Reverb>("Reverb");
	registerComponent<ParticleEmitter>("ParticleEmitter");
	registerComponent<Strider>("Strider");
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
		LOG("COMPONENT MANAGER: trying to get a ComponentFactory ID: %s. Does not exist.\n", nameID.c_str());
		return nullptr;
	}
	return factories[nameID];
}