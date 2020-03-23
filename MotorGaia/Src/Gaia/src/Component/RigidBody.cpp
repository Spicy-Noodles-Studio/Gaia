#include "RigidBody.h"

#include <btBulletDynamicsCommon.h>

#include "GameObject.h"
#include "GaiaMotionState.h"
#include "ComponentData.h"
#include "PhysicsSystem.h"
#include "ComponentData.h"
#include <sstream>

RigidBody::RigidBody(GameObject* gameObject) : GaiaComponent(gameObject), body(nullptr), motionState(nullptr)
{

}

RigidBody::~RigidBody()
{
	PhysicsSystem::GetInstance()->deleteRigidBody(body);
}

void RigidBody::setRigidBody(float mass, RB_Shape shape, const Vector3& offset, const Vector3& dim, uint16_t myGroup, uint16_t collidesWith)
{
	motionState = new GaiaMotionState(gameObject->transform, offset);
	body = PhysicsSystem::GetInstance()->createRigidBody(mass, shape, motionState, gameObject->transform->getWorldScale() * dim, myGroup, collidesWith);
	body->setUserPointer(this);
	if (mass > 0) disableDeactivation();
}

void RigidBody::handleData(ComponentData* data)
{
	float mass = 1.0, damping = 0.0, friction = 0.0, restitution = 0.0, angularDamping = 0.0;
	RB_Shape shape;
	Vector3 off = Vector3(), dim = Vector3(1, 1, 1), gravity = PhysicsSystem::GetInstance()->getWorldGravity(), movConstraints = { 1,1,1 }, rotConstraints = { 1,1,1 };
	bool isTrigger = false, kinematic = false;

	for (auto prop : data->getProperties()) {
		std::stringstream ss(prop.second);

		if (prop.first == "shape") {
			if (prop.second == "Box")
				shape = BOX_RB_SHAPE;
			else if (prop.second == "Sphere")
				shape = SPHERE_RB_SHAPE;
			else
				LOG("RIGIDBODY: %s not valid rigidbody shape type\n", prop.second.c_str());
		}
		else if (prop.first == "mass") {
			ss >> mass;
		}
		else if (prop.first == "friction") {
			ss >> friction;
		}
		else if (prop.first == "restitution") {
			ss >> restitution;
		}
		else if (prop.first == "damping") {
			ss >> damping;
		}
		else if (prop.first == "angularDamping") {
			ss >> angularDamping;
		}
		else if (prop.first == "offset") {
			ss >> off.x >> off.y >> off.z;
		}
		else if (prop.first == "scale") {
			ss >> dim.x >> dim.y >> dim.z;
		}
		else if (prop.first == "trigger") {
			ss >> isTrigger;
		}
		else if (prop.first == "kinematic") {
			ss >> kinematic;
		}
		else if (prop.first == "gravity") {
			ss >> gravity.x >> gravity.y >> gravity.z;
		}
		else if (prop.first == "movementConstraints") {
			ss >> movConstraints.x >> movConstraints.y >> movConstraints.z;
		}
		else if (prop.first == "rotationConstraints") {
			ss >> rotConstraints.x >> rotConstraints.y >> rotConstraints.z;
		}
	}
	setRigidBody(mass, shape, off, dim);

	setGravity(gravity);
	setDamping(damping);
	setAngularDamping(angularDamping);
	setFriction(friction);
	setRestitution(restitution);
	setMovementConstraints(movConstraints);
	setRotationConstraints(rotConstraints);

	setKinematic(kinematic);
	setTrigger(isTrigger);
}

void RigidBody::addForce(const Vector3& force, Vector3 relPos)
{
	if (relPos == Vector3(0.0f, 0.0f, 0.0f))
		body->applyCentralForce(parseToBulletVector(force));
	else
		body->applyForce(parseToBulletVector(force), parseToBulletVector(relPos));
}

void RigidBody::addImpulse(const Vector3& impulse, ImpulseMode mode, Vector3 relPos)
{
	switch (mode)
	{
	case IMPULSE:
		if (relPos == Vector3(0.0f, 0.0f, 0.0f))
			body->applyCentralImpulse(parseToBulletVector(impulse));
		else
			body->applyImpulse(parseToBulletVector(impulse), parseToBulletVector(relPos));
		break;
	case PUSH:
		body->applyPushImpulse(parseToBulletVector(impulse), parseToBulletVector(relPos));
		break;
	case TORQUE:
		body->applyTorqueImpulse(parseToBulletVector(impulse));
		break;
	case TORQUE_TURN:
		body->applyTorqueTurnImpulse(parseToBulletVector(impulse));
		break;
	}
}

void RigidBody::addTorque(const Vector3& torque)
{
	body->applyTorque(parseToBulletVector(torque));
}

void RigidBody::setGravity(const Vector3& grav)
{
	body->setGravity(parseToBulletVector(grav));
}

void RigidBody::setDamping(float damping)
{
	body->setDamping(btScalar(damping), body->getAngularDamping());
}

void RigidBody::setAngularDamping(float damping)
{
	body->setDamping(body->getLinearDamping(), btScalar(damping));
}

void RigidBody::setAngularVelocity(const Vector3& angVel)
{
	body->setAngularVelocity(parseToBulletVector(angVel));
}

void RigidBody::setLinearVelocity(const Vector3& vel)
{
	body->setLinearVelocity(parseToBulletVector(vel));
}

void RigidBody::setFriction(float friction)
{
	body->setFriction(friction);
}

void RigidBody::setRestitution(float restitution)
{
	body->setRestitution(restitution);
}

// 1 to allow movement 0 to add a constraint
void RigidBody::setMovementConstraints(const Vector3& constraints)
{
	body->setLinearFactor(parseToBulletVector(constraints));
}

// 1 to allow rotation 0 to add a constraint
void RigidBody::setRotationConstraints(const Vector3& constraints)
{
	body->setAngularFactor(parseToBulletVector(constraints));
}

void RigidBody::setTrigger(bool trigger)
{
	int flag = 0;
	if (isTrigger() && !trigger) flag = -body->CF_NO_CONTACT_RESPONSE;
	else if (!isTrigger() && trigger) flag = body->CF_NO_CONTACT_RESPONSE;
	body->setCollisionFlags(body->getCollisionFlags() + flag);
}

void RigidBody::setKinematic(bool kinematic)
{
	int flag = 0;
	if (isKinematic() && !kinematic) flag = -body->CF_KINEMATIC_OBJECT;
	else if (!isKinematic() && kinematic) flag = body->CF_KINEMATIC_OBJECT;
	body->setCollisionFlags(body->getCollisionFlags() + flag);
}

void RigidBody::setStatic(bool stat)
{
	int flag = 0;
	if (isStatic() && !stat) flag = -body->CF_STATIC_OBJECT;
	else if (!isStatic() && stat && body->getMass() > 0) flag = body->CF_STATIC_OBJECT;
	body->setCollisionFlags(body->getCollisionFlags() + flag);
}

void RigidBody::setActive(bool active)
{
	Component::setActive(active);

	int state = (active) ? ACTIVE_TAG : DISABLE_SIMULATION;
	body->forceActivationState(state);
}

// Multiplies original rigidBody scale with the input vector
void RigidBody::multiplyScale(const Vector3& scale)
{
	body->getCollisionShape()->setLocalScaling(parseToBulletVector(scale));
}

void RigidBody::updateTransform()
{
	if (!isKinematic()) {
		setKinematic(true);
		body->getMotionState()->getWorldTransform(body->getWorldTransform());
		setKinematic(false);
	}
}

void RigidBody::disableDeactivation()
{
	body->setActivationState(DISABLE_DEACTIVATION);
}

bool RigidBody::isTrigger() const
{
	return (body->getCollisionFlags() & body->CF_NO_CONTACT_RESPONSE) != 0;
}

bool RigidBody::isKinematic() const
{
	return body->isKinematicObject();
}

bool RigidBody::isStatic() const
{
	return body->isStaticObject();
}

float RigidBody::getLinearDamping() const
{
	return body->getLinearDamping();
}

float RigidBody::getFriction() const
{
	return body->getFriction();
}

float RigidBody::getRestitution() const
{
	return body->getRestitution();
}

const Vector3& RigidBody::getGravity() const
{
	return parseFromBulletVector(body->getGravity());
}

const Vector3& RigidBody::getAngularVelocity() const
{
	return parseFromBulletVector(body->getAngularVelocity());
}

const Vector3& RigidBody::getLinearVelocity() const
{
	return parseFromBulletVector(body->getLinearVelocity());
}

const Vector3& RigidBody::getTotalForce() const
{
	return parseFromBulletVector(body->getTotalForce());
}

const Vector3& RigidBody::getTotalTorque() const
{
	return parseFromBulletVector(body->getTotalTorque());
}

const Vector3& RigidBody::getOrientation() const
{
	btQuaternion btq = body->getOrientation();
	btScalar x, y, z;
	btq.getEulerZYX(z, y, x);
	return { x,y,z };
}



const btVector3 RigidBody::parseToBulletVector(const Vector3& v) const
{
	return btVector3(btScalar(v.x), btScalar(v.y), btScalar(v.z));
}

const Vector3 RigidBody::parseFromBulletVector(const btVector3& v) const
{
	return Vector3(double(v.x()), double(v.y()), double(v.z()));
}
