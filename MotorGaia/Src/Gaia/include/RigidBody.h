#pragma once
#ifndef RIGID_BODY_H
#define RIGID_BODY_H
#include "GaiaComponent.h"
#include "PhysicsSystem.h"

class GaiaMotionState;

class RigidBody : public GaiaComponent
{
private:
	btRigidBody* body = nullptr;
	static Vector3 btScaleConversion;
	GaiaMotionState* motionState;

public:
	RigidBody(GameObject* gameObject);
	~RigidBody();

	void setRigidBody(float mass, RB_Shape shape, Vector3 offset = { 0.0f, 0.0f, 0.0f });
};

#endif
