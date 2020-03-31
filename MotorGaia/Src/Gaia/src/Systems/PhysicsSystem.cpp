#include "PhysicsSystem.h"

#include <Bullet3Common/b3Scalar.h>
#include <btBulletDynamicsCommon.h>

#include "Transform.h"
#include "GaiaMotionState.h"
#include "RigidBody.h"
#include "GameObject.h"
#include "DebugDrawer.h"
#include "MeshStrider.h"


PhysicsSystem::PhysicsSystem() : dynamicsWorld(nullptr), collisionConfiguration(nullptr),
								 dispatcher(nullptr), overlappingPairCache(nullptr), solver(nullptr), timeAccumulator(0.0)
{
}

PhysicsSystem::~PhysicsSystem()
{
}

void PhysicsSystem::init()
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

	dynamicsWorld->setGravity(btVector3(0, -9.81, 0));

	dynamicsWorld->setForceUpdateAllAabbs(false);

	///-----initialization_end-----
	
}

void PhysicsSystem::render()
{
	dynamicsWorld->getDebugDrawer()->setDebugMode(btIDebugDraw::DBG_DrawWireframe);
	dynamicsWorld->debugDrawWorld();
}

void PhysicsSystem::update(float deltaTime)
{
	timeAccumulator += deltaTime;
	while (timeAccumulator >= 1.0f / 50.0f)
	{
		dynamicsWorld->stepSimulation(1.0f / 50.0f, 0);
		checkCollisions();
		timeAccumulator -= 1.0f / 50.0f;
	}
}

void PhysicsSystem::postUpdate()
{
	for (int i = dynamicsWorld->getNumCollisionObjects() - 1; i >= 0; i--)
	{
		btCollisionObject* obj = dynamicsWorld->getCollisionObjectArray()[i];
		btRigidBody* body = btRigidBody::upcast(obj);
		if (body != nullptr)
		{
			RigidBody* rb = (RigidBody*)body->getUserPointer();
			if (rb != nullptr && !rb->isStatic())
				rb->updateTransform();
		}
	}
}

void PhysicsSystem::close()
{
	///-----cleanup_start-----
	clearWorld();

	//delete dynamics world
	delete dynamicsWorld;

	//delete solver
	delete solver;

	//delete broadphase
	delete overlappingPairCache;

	//delete dispatcher
	delete dispatcher;

	delete collisionConfiguration;

	destroy();
}

void PhysicsSystem::clearWorld()
{
	//remove the rigidbodies from the dynamics world and delete them
	for (int i = dynamicsWorld->getNumCollisionObjects() - 1; i >= 0; i--)
	{
		btCollisionObject* obj = dynamicsWorld->getCollisionObjectArray()[i];
		deleteBody(obj);
	}

	//delete collision shapes
	for (int j = 0; j < collisionShapes.size(); j++)
	{
		btCollisionShape* shape = collisionShapes[j];
		collisionShapes[j] = nullptr;
		delete shape;
	}
	collisionShapes.clear();
}


void PhysicsSystem::setWorldGravity(Vector3 gravity)
{
	dynamicsWorld->setGravity(btVector3(gravity.x, gravity.y, gravity.z));
}

Vector3 PhysicsSystem::getWorldGravity() const
{
	btVector3 g = dynamicsWorld->getGravity();
	return{ g.x(),g.y(),g.z() };
}

void PhysicsSystem::setDebugDrawer(DebugDrawer* debugDrawer)
{
	debugDrawer->setDebugMode(btIDebugDraw::DBG_MAX_DEBUG_DRAW_MODE + btIDebugDraw::DBG_DrawContactPoints);
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
		colShape = new btBoxShape(btVector3(btScalar(dim.x / 2.0f), btScalar(dim.y / 2.0f), btScalar(dim.z / 2.0f)));
		break;
	case SPHERE_RB_SHAPE:
		colShape = new btSphereShape(btScalar(std::max(std::max(dim.x / 2.0f, dim.y / 2.0f), dim.z / 2.0f)));
		break;
	case CAPSULE_RB_SHAPE:
		colShape = new btCapsuleShape(btScalar(std::max(dim.x / 2.0f, dim.z / 2.0f)), btScalar(dim.y));
		break;
	case CYLINDER_RB_SHAPE:
		colShape = new btCylinderShape(btVector3(btScalar(dim.x / 2.0f), btScalar(dim.y / 2.0f), btScalar(dim.z / 2.0f)));
		break;
	case CONE_RB_SHAPE:
		colShape = new btConeShape(btScalar(std::max(dim.x / 2.0f, dim.z / 2.0f)), btScalar(dim.y));
		break;
	default:
		break;
	}

	collisionShapes.push_back(colShape);

	/// Create Dynamic Objects
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

void PhysicsSystem::deleteRigidBody(btRigidBody* body)
{
	if (body != nullptr) {

		RigidBody* rigidBody = (RigidBody*)body->getUserPointer();
		//Buscar en la lista de contactos
		//Registramos presencia
		std::vector<std::pair<RigidBody*, RigidBody*>> presence;
		for (auto contact : contacts) {
			if (contact.first.first == rigidBody || contact.first.second == rigidBody)
				presence.push_back(contact.first);
		}

		for (auto contact : presence) {
			//TODO: a lo mejor llamar a OnExitCallback del que no se elimina??
			contacts.erase(contact);
		}


		btCollisionObject* obj = body;
		btCollisionShape* shape = obj->getCollisionShape();
		deleteBody(obj);

		auto it = std::find(collisionShapes.begin(), collisionShapes.end(), shape);
		if (it != collisionShapes.end())
			collisionShapes.erase(it);
		delete shape;
	}
}

btTransform PhysicsSystem::parseToBulletTransform(Transform* transform)
{
	btTransform t;
	t.setIdentity();
	Vector3 pos = transform->getWorldPosition(), rot = transform->getWorldRotation();
	t.setOrigin({ btScalar(pos.x), btScalar(pos.y), btScalar(pos.z) });
	btQuaternion quat = (btQuaternion(btScalar(rot.z) * SIMD_RADS_PER_DEG, btScalar(rot.y) * SIMD_RADS_PER_DEG, btScalar(rot.x) * SIMD_RADS_PER_DEG)); quat.normalize();
	t.setRotation(quat);
	return t;
}

btRigidBody* PhysicsSystem::bodyFromStrider(MeshStrider* strider, GaiaMotionState* mState, const Vector3& dim)
{
	btCollisionShape* colShape = new btBvhTriangleMeshShape(strider, true, true);
	collisionShapes.push_back(colShape);
	colShape->setLocalScaling({ btScalar(dim.x), btScalar(dim.y), btScalar(dim.z) });

	btScalar mass = 0; //Always static
	btVector3 localInertia(0, 0, 0);

	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, mState, colShape, localInertia);
	btRigidBody* body = new btRigidBody(rbInfo);

	dynamicsWorld->addRigidBody(body);

	return body;
}

void PhysicsSystem::checkCollisions()
{
	std::map<std::pair<RigidBody*, RigidBody*>, bool> newContacts;

	int numManifolds = dynamicsWorld->getDispatcher()->getNumManifolds();
	for (int i = 0; i < numManifolds; i++)
	{
		btPersistentManifold* contactManifold = dynamicsWorld->getDispatcher()->getManifoldByIndexInternal(i);

		//Comprobamos que realmente hay contacto
		int numContacts = contactManifold->getNumContacts(), j = 0;
		bool contact = false;
		btManifoldPoint pt;
		btCollisionObject* obA = (btCollisionObject*)(contactManifold->getBody0()), * obB = (btCollisionObject*)(contactManifold->getBody1());

		btVector3 aux;
		btScalar dim1, dim2;
		obA->getCollisionShape()->getBoundingSphere(aux, dim1);
		obB->getCollisionShape()->getBoundingSphere(aux, dim2);
		dim1 = std::min(dim1, dim2);

		while (!contact && j < numContacts)
		{
			pt = contactManifold->getContactPoint(j);
			contact = pt.getDistance() < 0.f && pt.getDistance()> -dim1;
			j++;
		}

		if (contact)
		{
			RigidBody* rbA = (RigidBody*)obA->getUserPointer(), * rbB = (RigidBody*)obB->getUserPointer();

			if (!(rbA == nullptr || rbB == nullptr))
			{
				// Orden A < B, para el mapa
				if (rbA > rbB) std::swap(rbA, rbB);

				std::pair<RigidBody*, RigidBody*> col = { rbA,rbB };
				newContacts[col] = true;

				//Llamamos al collisionEnter si no estaban registrados.
				if (contacts.find(col) == contacts.end())
					CollisionEnterCallbacks(col);
				// Si ya estaban llamamos al collisionStay.
				else
					CollisionStayCallbacks(col);
			}
		}
	}

	//Comprobamos que contactos ya no estan
	for (auto it = contacts.begin(); it != contacts.end(); it++)
	{
		std::pair<RigidBody*, RigidBody*> col = (*it).first;
		if (newContacts.find(col) == newContacts.end())
			CollisionExitCallbacks(col);
	}

	contacts = newContacts;
}

void PhysicsSystem::CollisionEnterCallbacks(const std::pair<RigidBody*, RigidBody*>& col)
{
	bool aTrigger = col.first->isTrigger(), bTrigger = col.second->isTrigger();
	GameObject* goA = col.first->gameObject, * goB = col.second->gameObject;

	if (!aTrigger && !bTrigger) {
		goA->onCollisionEnter(goB);
		goB->onCollisionEnter(goA);
	}
	else if (aTrigger && !bTrigger) {
		goB->onTriggerEnter(goA);
		goA->onObjectEnter(goB);
	}
	else if (bTrigger && !aTrigger) {
		goA->onTriggerEnter(goB);
		goB->onObjectEnter(goA);
	}
}

void PhysicsSystem::CollisionExitCallbacks(const std::pair<RigidBody*, RigidBody*>& col)
{
	if (col.first->gameObject == nullptr || col.second->gameObject == nullptr)
		return;

	bool aTrigger = col.first->isTrigger(), bTrigger = col.second->isTrigger();
	GameObject* goA = col.first->gameObject, * goB = col.second->gameObject;

	if (!aTrigger && !bTrigger) {
		goA->onCollisionExit(goB);
		goB->onCollisionExit(goA);
	}
	else if (aTrigger && !bTrigger) {
		goB->onTriggerExit(goA);
		goA->onObjectExit(goB);
	}
	else if (bTrigger && !aTrigger) {
		goA->onTriggerExit(goB);
		goB->onObjectExit(goA);
	}
}

void PhysicsSystem::CollisionStayCallbacks(const std::pair<RigidBody*, RigidBody*>& col)
{
	bool aTrigger = col.first->isTrigger(), bTrigger = col.second->isTrigger();
	GameObject* goA = col.first->gameObject, * goB = col.second->gameObject;

	if (!aTrigger && !bTrigger) {
		goA->onCollisionStay(goB);
		goB->onCollisionStay(goA);
	}
	else if (aTrigger && !bTrigger) {
		goB->onTriggerStay(goA);
		goA->onObjectStay(goB);
	}
	else if (bTrigger && !aTrigger) {
		goA->onTriggerStay(goB);
		goB->onObjectStay(goA);
	}
}

void PhysicsSystem::deleteBody(btCollisionObject* obj)
{
	btRigidBody* body = btRigidBody::upcast(obj);
	if (body != nullptr && body->getMotionState() != nullptr)
	{
		delete body->getMotionState();
	}
	dynamicsWorld->removeCollisionObject(obj);
	delete obj;
}
