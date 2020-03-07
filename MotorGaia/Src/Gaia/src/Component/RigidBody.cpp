#include "RigidBody.h"
#include "btBulletDynamicsCommon.h"
#include "GameObject.h"
#include "GaiaMotionState.h"
#include "ComponentData.h"

Vector3 RigidBody::btScaleConversion = { 50,50,50 };

RigidBody::RigidBody(GameObject* gameObject) : GaiaComponent(gameObject)
{
}


RigidBody::~RigidBody()
{
}

void RigidBody::setRigidBody(float mass, RB_Shape shape, const Vector3& offset, const Vector3& dim, bool isT)
{
	motionState = new GaiaMotionState(gameObject->transform, offset);
	body = PhysicsSystem::GetInstance()->createRigidBody(mass, shape, motionState, gameObject->transform->getScale() * dim * btScaleConversion);
	body->setCollisionFlags(body->getCollisionFlags() | (body->CF_NO_CONTACT_RESPONSE * isT));
	body->setUserPointer(this);
	trigger = isT;
}

void RigidBody::handleData(ComponentData* data)
{
	float mass = 1.0;
	RB_Shape shape;
	Vector3 off = Vector3(), dim = Vector3(1,1,1);
	bool isTrigger = false;

	for (auto prop : data->getProperties()) {
		std::stringstream ss(prop.second);

		if (prop.first == "shape") {
			if (prop.second == "Box")
				shape = BOX_RB_SHAPE;
			else if (prop.second == "Sphere")
				shape = SPHERE_RB_SHAPE;
			else
				printf("RIGIDBODY: %s not valid rigidbody shape type\n", prop.second.c_str());
		}
		else if (prop.first == "mass") {
			ss >> mass;
		}
		else if (prop.first == "offset") {
			ss >> off.x >> off.y >> off.z;
		}
		else if (prop.first == "size") {
			ss >> dim.x >> dim.y >> dim.z;
		}
		else if (prop.first == "trigger") {
			ss >> isTrigger;
		}
	}
	setRigidBody(mass, shape, off, dim, isTrigger);
}

bool RigidBody::isTrigger() const
{
	return trigger;
}
