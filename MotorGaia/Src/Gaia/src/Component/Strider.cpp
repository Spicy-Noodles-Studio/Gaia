#include "Strider.h"

#include "MeshStrider.h"
#include "MeshRenderer.h"
#include "PhysicsSystem.h"
#include "GameObject.h"
#include "OgreEntity.h"

Strider::Strider(GameObject* gameObject) : RigidBody(gameObject), meshStrider(nullptr)
{
}

Strider::~Strider()
{
	delete meshStrider;
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
		Ogre::Entity* ent = mRend->getMesh(mesh);
		if (ent != nullptr) {
			Ogre::MeshPtr mPtr = ent->getMesh();
			if (meshStrider != nullptr) delete meshStrider;
			meshStrider = new MeshStrider(mPtr.getPointer());
			PhysicsSystem::GetInstance()->bodyFromStrider(meshStrider, gameObject->transform->getWorldPosition(), gameObject->transform->getWorldScale());
		}
		else
			LOG("STRIDER: Gameobject %s does not have an Entity called.\n", mesh.c_str());
	}
	else
		LOG("STRIDER: Gameobject %s does not have a MeshRenderer component.\n", gameObject->getName().c_str());
}
