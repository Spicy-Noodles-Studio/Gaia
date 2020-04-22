#pragma once

#ifndef RAYCAST_HIT_H
#define RAYCAST_HIT_H

#include "GaiaExports.h"
#include "Vector3.h"

class Transform;
class RigidBody;

struct GAIA_API RaycastHit
{
friend class PhysicsSystem;
public:
	RaycastHit();

	RigidBody* rb;
	Transform* transform;
	Vector3 point, normal;
	float distance;
private:
	void createRaycastHit(RigidBody* rb, const btVector3& normal, const btVector3& point, float distance);
};

#endif 