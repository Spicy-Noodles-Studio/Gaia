#include "RigidBody.h"
#include "btBulletDynamicsCommon.h"


RigidBody::RigidBody(GameObject* gameObject, float mass, RB_Shape shape, Vector3 dimensions, Vector3 position) : GaiaComponent(gameObject)
{
	body = PhysicsSystem::GetInstance()->createRigidBody(mass, shape, dimensions, position);
}


RigidBody::~RigidBody()
{
	
}
