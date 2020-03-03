#pragma once
#ifndef PHYSICS_SYSTEM_H
#define PHYSICS_SYSTEM_H

#include "Singleton.h"
#include "btBulletDynamicsCommon.h"

class PhysicsSystem : public Singleton<PhysicsSystem> 
{
public:
	PhysicsSystem();
	~PhysicsSystem();

	void setup();
	void update();
	void shutDown();

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