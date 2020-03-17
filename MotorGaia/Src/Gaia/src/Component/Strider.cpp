#include "Strider.h"

#include "MeshStrider.h"
#include "MeshRenderer.h"
#include "PhysicsSystem.h"
#include "GameObject.h"
#include "OgreEntity.h"

Strider::Strider(GameObject* gameObject) : GaiaComponent(gameObject)
{
}

void Strider::handleData(ComponentData* data)
{
	for (auto prop : data->getProperties())
	{

		if (prop.first == "mesh")
		{
			stride(prop.second);
		}
		else
			LOG("STRIDER:  property %s does not exist\n", prop.first.c_str());
	}
}

void Strider::stride(const std::string& mesh)
{
	MeshRenderer* mRend = gameObject->getComponent<MeshRenderer>();
	if (mRend != nullptr)
	{
		Ogre::MeshPtr mPtr= mRend->getMesh(mesh)->getMesh();
		MeshStrider* meshStrider = new MeshStrider(mPtr.getPointer());
		PhysicsSystem::GetInstance()->bodyFromStrider(meshStrider, gameObject->transform->getWorldPosition());
	}
	else
		LOG("STRIDER: Gameobject %s does not have a MeshRenderer component.\n", gameObject->getName().c_str());
}
