#include "RaycastHit.h"

#include "RigidBody.h"
#include "Transform.h"
#include "GameObject.h"

RaycastHit::RaycastHit(): transform(nullptr), rb(nullptr), distance(0.0f), normal({0,0,0}), point({0,0,0})
{

}

void RaycastHit::createRaycastHit(RigidBody* rb, const btVector3& normal, const btVector3& point, float distance)
{
	this->rb = rb;
	transform = rb->gameObject->transform;
	this->normal = rb->parseFromBulletVector(normal);
	this->point = rb->parseFromBulletVector(point);
}
