#pragma once
#ifndef PHYSICS_SYSTEM_H
#define PHYSICS_SYSTEM_H

#include <map>
#include <vector>

#include "Vector3.h"
#include "Singleton.h"
#include "PhysicsUtils.h"

class DebugDrawer;
class Transform;
class GaiaMotionState;
class RigidBody;
class MeshStrider;

class btRigidBody;
class btTransform;
class btCollisionObject;
class btDiscreteDynamicsWorld;
class btDefaultCollisionConfiguration;
class btCollisionDispatcher;
class btBroadphaseInterface;
class btSequentialImpulseConstraintSolver;
class btCollisionShape;

class GAIA_API PhysicsSystem : public Singleton<PhysicsSystem>
{
public:
	PhysicsSystem();
	~PhysicsSystem();

	void init();
	void render();
	void update(float deltaTime);
	void postUpdate();
	void close();

	void clearWorld();

	// World config methods
	void setWorldGravity(Vector3 gravity);
	Vector3 getWorldGravity()const;

	void setDebugDrawer(DebugDrawer* debugDrawer);

	// Rigid Body methods
	btRigidBody* createRigidBody(float m, RB_Shape shape, GaiaMotionState* mState, Vector3 dim, uint16_t myGroup=NONE, uint16_t mask=ALL);
	void deleteRigidBody(btRigidBody* body);
	// Turns a Gaia Transform into a Bullet Physics Transform
	btTransform parseToBulletTransform(Transform* transform);
	btRigidBody* bodyFromStrider(MeshStrider* strider, GaiaMotionState* mState, const Vector3& dim);

	void checkCollisions();

private:
	void CollisionEnterCallbacks(const std::pair<RigidBody*, RigidBody*>& col);
	void CollisionExitCallbacks(const std::pair<RigidBody*, RigidBody*>& col);
	void CollisionStayCallbacks(const std::pair<RigidBody*, RigidBody*>& col);

	void deleteBody(btCollisionObject* obj);

	btDiscreteDynamicsWorld* dynamicsWorld;
	btDefaultCollisionConfiguration* collisionConfiguration;
	btCollisionDispatcher* dispatcher;
	btBroadphaseInterface* overlappingPairCache;
	btSequentialImpulseConstraintSolver* solver;

	//keep track of the shapes, we release memory at exit.
	//make sure to re-use collision shapes among rigid bodies whenever possible!
	std::vector<btCollisionShape*> collisionShapes;

	std::map<std::pair<RigidBody*, RigidBody*>, bool> contacts;


	float time;
};

#endif