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
	~RaycastHit();

	RigidBody* rigidBody;
	Transform* transform;
	Vector3 point, normal;
	float distance;
private:
	void createRaycastHit(RigidBody* rigidBody, const btVector3& normal, const btVector3& point, float distance);
};

#endif 