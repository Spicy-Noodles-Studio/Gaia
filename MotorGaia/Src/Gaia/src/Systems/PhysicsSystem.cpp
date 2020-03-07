#include "PhysicsSystem.h"

#include "Bullet3Common/b3Scalar.h"

#include "Transform.h"
#include "GaiaMotionState.h"
#include "RigidBody.h"
#include "GameObject.h"
#include "gTime.h"

PhysicsSystem::PhysicsSystem()
{
}

PhysicsSystem::~PhysicsSystem()
{
}

void PhysicsSystem::setup()
{
	///-----initialization_start-----

	///collision configuration contains default setup for memory, collision setup. Advanced users can create their own configuration.
	collisionConfiguration = new btDefaultCollisionConfiguration();

	///use the default collision dispatcher. For parallel processing you can use a diffent dispatcher (see Extras/BulletMultiThreaded)
	dispatcher = new btCollisionDispatcher(collisionConfiguration);

	///btDbvtBroadphase is a good general purpose broadphase. You can also try out btAxis3Sweep.
	overlappingPairCache = new btDbvtBroadphase();

	///the default constraint solver. For parallel processing you can use a different solver (see Extras/BulletMultiThreaded)
	solver = new btSequentialImpulseConstraintSolver;

	dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, overlappingPairCache, solver, collisionConfiguration);

	dynamicsWorld->setGravity(btVector3(0, -10, 0));

	///-----initialization_end-----
}

void PhysicsSystem::update()
{
	dynamicsWorld->stepSimulation(1.0f / 50.0f, 10);
	dynamicsWorld->debugDrawWorld();
	checkCollisions();
}

void PhysicsSystem::shutDown()
{
	///-----cleanup_start-----

	//remove the rigidbodies from the dynamics world and delete them
	for (int i = dynamicsWorld->getNumCollisionObjects() - 1; i >= 0; i--)
	{
		btCollisionObject* obj = dynamicsWorld->getCollisionObjectArray()[i];
		btRigidBody* body = btRigidBody::upcast(obj);
		if (body && body->getMotionState())
		{
			delete body->getMotionState();
		}
		dynamicsWorld->removeCollisionObject(obj);
		delete obj;
	}

	//delete collision shapes
	for (int j = 0; j < collisionShapes.size(); j++)
	{
		btCollisionShape* shape = collisionShapes[j];
		collisionShapes[j] = 0;
		delete shape;
	}

	//delete dynamics world
	delete dynamicsWorld;

	//delete solver
	delete solver;

	//delete broadphase
	delete overlappingPairCache;

	//delete dispatcher
	delete dispatcher;

	delete collisionConfiguration;

	//next line is optional: it will be cleared by the destructor when the array goes out of scope
	collisionShapes.clear();
}


void PhysicsSystem::setWorldGravity(Vector3 gravity)
{
	dynamicsWorld->setGravity(btVector3(gravity.x, gravity.y, gravity.z));
}

void PhysicsSystem::setDebugDrawer(DebugDrawer* debugDrawer)
{
	debugDrawer->setDebugMode(btIDebugDraw::DBG_MAX_DEBUG_DRAW_MODE);
	dynamicsWorld->setDebugDrawer(debugDrawer);
}



// Creates a btRigidBody with the specified properties, adds it to the dynamicWorld
// and returns a reference to it
btRigidBody* PhysicsSystem::createRigidBody(float m, RB_Shape shape, GaiaMotionState* mState, Vector3 dim)
{
	btCollisionShape* colShape;
	switch (shape)
	{
	case BOX_RB_SHAPE:
		colShape = new btBoxShape(btVector3(btScalar(dim.x), btScalar(dim.y), btScalar(dim.z)));
		break;
	case SPHERE_RB_SHAPE:
		colShape = new btSphereShape(btScalar(std::max(std::max(dim.x, dim.y), dim.z)));
		break;
	default:
		break;
	}

	collisionShapes.push_back(colShape);

	/// Create Dynamic Objects
	//btTransform transform = parseToBulletTransform(mState->getTransform());
	//	transform.setOrigin(btVector3(pos.x, pos.y, pos.z));

	btScalar mass(m);

	//rigidbody is dynamic if and only if mass is non zero, otherwise static
	bool isDynamic = (mass != 0.f);

	btVector3 localInertia(0, 0, 0);
	if (isDynamic)
		colShape->calculateLocalInertia(mass, localInertia);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, mState, colShape, localInertia);
	btRigidBody* body = new btRigidBody(rbInfo);

	dynamicsWorld->addRigidBody(body);

	return body;
}

btTransform PhysicsSystem::parseToBulletTransform(Transform* transform)
{
	btTransform t;
	t.setIdentity();
	t.setOrigin({ btScalar(transform->getPosition().x), btScalar(transform->getPosition().y), btScalar(transform->getPosition().z) });
	t.setRotation(btQuaternion(btScalar(transform->getRotation().y) * SIMD_RADS_PER_DEG, btScalar(transform->getRotation().x) * SIMD_RADS_PER_DEG, btScalar(transform->getRotation().z) * SIMD_RADS_PER_DEG));
	return t;
}

void PhysicsSystem::checkCollisions()
{
	std::map<std::pair<RigidBody*, RigidBody*>, bool> newContacts;

	int numManifolds = dynamicsWorld->getDispatcher()->getNumManifolds();
	for (int i = 0; i < numManifolds; i++)
	{
		btPersistentManifold* contactManifold = dynamicsWorld->getDispatcher()->getManifoldByIndexInternal(i);
		btCollisionObject* obA = (btCollisionObject*)(contactManifold->getBody0());
		btCollisionObject* obB = (btCollisionObject*)(contactManifold->getBody1());

		RigidBody* rbA = (RigidBody*)obA->getUserPointer();
		RigidBody* rbB = (RigidBody*)obB->getUserPointer();

		if (rbA == 0 or rbB == 0)
			return;

		// Orden A < B, para el mapa
		RigidBody* aux;
		if (rbA > rbB)
		{
			aux = rbA;
			rbA = rbB;
			rbB = aux;
		}

		GameObject* goA = rbA->gameObject;
		GameObject* goB = rbB->gameObject;



		//Saca los puntos de colision
		/*int numContacts = contactManifold->getNumContacts();
		for (int j = 0; j < numContacts; j++)
		{
			btManifoldPoint& pt = contactManifold->getContactPoint(j);
			if (pt.getDistance() < 0.f)
			{
				const btVector3& ptA = pt.getPositionWorldOnA();
				const btVector3& ptB = pt.getPositionWorldOnB();
				const btVector3& normalOnB = pt.m_normalWorldOnB;
			}
		}*/



		newContacts[{rbA, rbB}] = true;

		bool aTrigger = rbA->isTrigger(), bTrigger = rbB->isTrigger();

		//llamamos al collisionEnter si no estaban registrados.
		if (!contacts[{rbA, rbB}])
		{
			if (!aTrigger && !bTrigger) {
				goA->onCollisionEnter(goB);
				goB->onCollisionEnter(goA);
			}
			else if (aTrigger && !bTrigger)
				goB->onTriggerEnter(goA);
			else if (bTrigger && !aTrigger)
				goA->onTriggerEnter(goB);
		}// Si ya estaban llamamos al collisionStay.
		else {
			if (!aTrigger && !bTrigger) {
				goA->onCollisionStay(goB);
				goB->onCollisionStay(goA);
			}
			else if (aTrigger && !bTrigger)
				goB->onTriggerStay(goA);
			else if (bTrigger && !aTrigger)
				goA->onTriggerStay(goB);
		}

	}

	for (auto it = contacts.begin(); it != contacts.end(); it++)
	{
		std::pair<RigidBody*, RigidBody*> col = (*it).first;
		if (newContacts.find(col) == newContacts.end())
		{
			bool aTrigger = col.first->isTrigger();
			bool bTrigger = col.second->isTrigger();
			GameObject* goA = col.first->gameObject;
			GameObject* goB = col.second->gameObject;

			if (!aTrigger && !bTrigger) {
				goA->onCollisionExit(goB);
				goB->onCollisionExit(goA);
			}
			else if (aTrigger && !bTrigger)
				goB->onTriggerExit(goA);
			else if (bTrigger && !aTrigger)
				goA->onTriggerExit(goB);
		}
	}

	contacts = newContacts;
}
