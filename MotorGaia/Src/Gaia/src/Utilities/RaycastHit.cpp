#include "RaycastHit.h"

#include "RigidBody.h"
#include "Transform.h"
#include "GameObject.h"

RaycastHit::RaycastHit(): transform(nullptr), rigidBody(nullptr), distance(0.0f), normal({0,0,0}), point({0,0,0})
{

}

RaycastHit::~RaycastHit()
{

}

void RaycastHit::createRaycastHit(RigidBody* rigidBody, const btVector3& normal, const btVector3& point, float distance)
{
	checkNullAndBreak(rigidBody);
	checkNullAndBreak(rigidBody->gameObject);
	checkNullAndBreak(rigidBody->gameObject->transform);

	this->rigidBody = rigidBody;
	transform = rigidBody->gameObject->transform;
	this->normal = rigidBody->parseFromBulletVector(normal);
	this->normal.normalize();
	this->point = rigidBody->parseFromBulletVector(point);
}
