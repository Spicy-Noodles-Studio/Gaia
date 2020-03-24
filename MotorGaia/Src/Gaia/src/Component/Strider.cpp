#include "Strider.h"

#include <sstream>
#include "MeshStrider.h"
#include "MeshRenderer.h"
#include "PhysicsSystem.h"
#include "GameObject.h"
#include "OgreEntity.h"
#include "GaiaMotionState.h"

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
		float friction = 0.0f;
		std::stringstream ss(prop.second);
		if (prop.first == "mesh")
		{
			stride(prop.second);
		}
		else if (prop.first == "friction") {
			if (!(ss >> friction))
				LOG("STRIDER: wrong value for property %s.\n", prop.first.c_str());
		}
		else
			LOG("STRIDER: property %s does not exist\n", prop.first.c_str());
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
			motionState = new GaiaMotionState(gameObject->transform);

			body = PhysicsSystem::GetInstance()->bodyFromStrider(meshStrider, motionState, gameObject->transform->getWorldScale());
			body->setUserPointer(this);
		}
		else
			LOG("STRIDER: Gameobject %s does not have an Entity called.\n", mesh.c_str());
	}
	else
		LOG("STRIDER: Gameobject %s does not have a MeshRenderer component.\n", gameObject->getName().c_str());
}
