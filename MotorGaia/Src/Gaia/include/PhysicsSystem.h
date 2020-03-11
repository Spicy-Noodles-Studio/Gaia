#pragma once
#ifndef PHYSICS_SYSTEM_H
#define PHYSICS_SYSTEM_H

#include "btBulletDynamicsCommon.h"
#include <map>

#include "Vector3.h"
#include "Singleton.h"
#include"DebugDrawer.h"

class Transform;
class GaiaMotionState;
class RigidBody;

enum RB_Shape
{
	BOX_RB_SHAPE, SPHERE_RB_SHAPE
};

//Potencias de 2 hasta 2^15
enum Col_Filters
{
	NONE = -1,
	DEFAULT = (1 << 0),
	ALL = (1 << 15)
};

class PhysicsSystem : public Singleton<PhysicsSystem> 
{
public:
	PhysicsSystem();
	~PhysicsSystem();

	void init();
	void render();
	void update();
	void close();

	void clearWorld();

	// World config methods
	void setWorldGravity(Vector3 gravity);

	void setDebugDrawer(DebugDrawer* debugDrawer);

	// Rigid Body methods
	btRigidBody* createRigidBody(float m, RB_Shape shape, GaiaMotionState* mState, Vector3 dim, uint16_t myGroup=DEFAULT, uint16_t mask=ALL);
	void deleteRigidBody(btRigidBody* body);
	// Turns a Gaia Transform into a Bullet Physics Transform
	btTransform parseToBulletTransform(Transform* transform);

	void checkCollisions();

private:
	btDiscreteDynamicsWorld* dynamicsWorld;
	btDefaultCollisionConfiguration* collisionConfiguration;
	btCollisionDispatcher* dispatcher;
	btBroadphaseInterface* overlappingPairCache;
	btSequentialImpulseConstraintSolver* solver;

	//keep track of the shapes, we release memory at exit.
	//make sure to re-use collision shapes among rigid bodies whenever possible!
	btAlignedObjectArray<btCollisionShape*> collisionShapes;

	std::map<std::pair<RigidBody*, RigidBody*>, bool> contacts;
};

#endif