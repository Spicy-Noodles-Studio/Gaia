#pragma once
#ifndef RIGID_BODY_H
#define RIGID_BODY_H
#include "GaiaComponent.h"
#include "PhysicsSystem.h"

class RigidBody : public GaiaComponent
{
public:
	btRigidBody* body = nullptr;


	RigidBody(GameObject* gameObject, float mass, RB_Shape shape, Vector3 dimensions, Vector3 position = { 0.0f, 0.0f, 0.0f });
	~RigidBody();


};

#endif
