#include "RigidBody.h"
#include "btBulletDynamicsCommon.h"
#include "GameObject.h"

Vector3 RigidBody::btScaleConversion = { 50,50,50 };

RigidBody::RigidBody(GameObject* gameObject) : GaiaComponent(gameObject)
{
}


RigidBody::~RigidBody()
{
}

void RigidBody::setRigidBody(float mass, RB_Shape shape, Vector3 offset)
{
	body = PhysicsSystem::GetInstance()->createRigidBody(mass, shape, gameObject->transform->getScale()*btScaleConversion, gameObject->transform->getPosition() + offset);
}
