#include "Strider.h"

#include <sstream>

#include "PhysicsSystem.h"
#include "GameObject.h"
#include "MeshRenderer.h"
#include "MeshStrider.h"
#include "OgreEntity.h"
#include "GaiaMotionState.h"
#include "ComponentRegister.h"

REGISTER_FACTORY(Strider);

Strider::Strider(GameObject *gameObject) : RigidBody(gameObject), meshStrider(nullptr), myGroup(DEFAULT), collidesWith(ALL)
{

}

Strider::~Strider()
{
	if(meshStrider != nullptr)
		delete meshStrider;
}

void Strider::handleData(ComponentData *data)
{
	checkNullAndBreak(data);

	float friction = 0.0f,restitution = 0.0, damping = 0.0f, angularDamping = 0.0f;
	bool trigger = false;
	for (auto prop : data->getProperties())
	{
		std::stringstream ss(prop.second);
		if (prop.first == "mesh")
		{
			stride(prop.second);
		}
		else if (prop.first == "friction")
		{
			setFloat(friction);
		}
		else if (prop.first == "trigger")
		{
			setBool(trigger);
		}
		else if (prop.first == "restitution") {
			setFloat(restitution);
		}
		else if (prop.first == "damping") {
			setFloat(damping);
		}
		else if (prop.first == "angularDamping") {
			setFloat(angularDamping);
		}
		else if (prop.first == "collisionGroup") {
			auto it = (colPresets.find(prop.second));
			if (it != colPresets.end())
				myGroup = (*it).second;
			else
				LOG_ERROR("STRIDER", "Wrong value for property %s", prop.first.c_str());
		}
		else if (prop.first == "collidesWith") {
			auto it = (colPresets.find(prop.second));
			if (it != colPresets.end())
				collidesWith = (*it).second;
			else
				LOG_ERROR("STRIDER", "Wrong value for property %s", prop.first.c_str());
		}
		else
			LOG_ERROR("STRIDER", "Property %s does not exist", prop.first.c_str());
	}
	if (body == nullptr) return;

	setFriction(friction);
	setTrigger(trigger);
	setDamping(damping);
	setAngularDamping(angularDamping);
	setFriction(friction);
	setRestitution(restitution);
}

void Strider::stride(const std::string &mesh)
{
	MeshRenderer *mRend = gameObject->getComponent<MeshRenderer>();
	if (mRend != nullptr)
	{
		Ogre::Entity *ent = mRend->getMesh(mesh);
		if (ent != nullptr)
		{
			Ogre::MeshPtr mPtr = ent->getMesh();
			if (meshStrider != nullptr)
				delete meshStrider;
			meshStrider = new MeshStrider(mPtr.getPointer());
			motionState = new GaiaMotionState(gameObject->transform);

			body = PhysicsSystem::GetInstance()->bodyFromStrider(meshStrider, motionState, gameObject->transform->getWorldScale(), myGroup, collidesWith);
			body->setUserPointer(this);
		}
		else
			LOG("STRIDER: Gameobject %s does not have an Entity called.\n", mesh.c_str());
	}
	else
		LOG("STRIDER: Gameobject %s does not have a MeshRenderer component.\n", gameObject->getName().c_str());
}
