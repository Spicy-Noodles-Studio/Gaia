#pragma once
#ifndef PHYSICS_SYSTEM_H
#define PHYSICS_SYSTEM_H

#include "Singleton.h"
#include "btBulletDynamicsCommon.h"
#include "Vector3.h"

enum RB_Shape
{
	BOX_RB_SHAPE, SPHERE_RB_SHAPE
};

class PhysicsSystem : public Singleton<PhysicsSystem> 
{
public:
	PhysicsSystem();
	~PhysicsSystem();

	void setup();
	void update();
	void shutDown();

	// World config methods
	void setWorldGravity(Vector3 gravity);

	// Rigid Body methods
	btRigidBody* createRigidBody(float m, RB_Shape shape, Vector3 dim, Vector3 pos);

private:
	btDiscreteDynamicsWorld* dynamicsWorld;
	btDefaultCollisionConfiguration* collisionConfiguration;
	btCollisionDispatcher* dispatcher;
	btBroadphaseInterface* overlappingPairCache;
	btSequentialImpulseConstraintSolver* solver;

	//keep track of the shapes, we release memory at exit.
	//make sure to re-use collision shapes among rigid bodies whenever possible!
	btAlignedObjectArray<btCollisionShape*> collisionShapes;
};

#endif