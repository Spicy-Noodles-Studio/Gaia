#include "RigidBody.h"

#include <btBulletDynamicsCommon.h>
#include <sstream>

#include "GameObject.h"
#include "GaiaMotionState.h"
#include "ComponentData.h"
#include "PhysicsSystem.h"
#include "ComponentData.h"
#include "ComponentRegister.h"

REGISTER_FACTORY(RigidBody);

RigidBody::RigidBody(GameObject* gameObject) : GaiaComponent(gameObject), body(nullptr), motionState(nullptr), colPresets()
{
	initPresets();
}

RigidBody::~RigidBody()
{
	PhysicsSystem* physicsSystem = PhysicsSystem::GetInstance();
	checkNullAndBreak(physicsSystem);

	physicsSystem->deleteRigidBody(body);
}

void RigidBody::setRigidBody(float mass, RB_Shape shape, const Vector3& offset, const Vector3& dim, uint16_t myGroup, uint16_t collidesWith)
{
	checkNullAndBreak(gameObject);
	checkNullAndBreak(gameObject->transform);

	motionState = new GaiaMotionState(gameObject->transform, offset);
	checkNullAndBreak(motionState);

	PhysicsSystem* physicsSystem = PhysicsSystem::GetInstance();
	checkNullAndBreak(physicsSystem);

	body = physicsSystem->createRigidBody(mass, shape, motionState, gameObject->transform->getWorldScale() * dim, myGroup, collidesWith);

	checkNullAndBreak(body);
	body->setUserPointer(this);
	if (mass > 0) disableDeactivation();
}

void RigidBody::addForce(const Vector3& force, Vector3 relPos)
{
	checkNullAndBreak(body);
	if (relPos == Vector3(0.0f, 0.0f, 0.0f))
		body->applyCentralForce(parseToBulletVector(force));
	else
		body->applyForce(parseToBulletVector(force), parseToBulletVector(relPos));
}

void RigidBody::addImpulse(const Vector3& impulse, ImpulseMode mode, Vector3 relPos)
{
	checkNullAndBreak(body);
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
	checkNullAndBreak(body);
	body->applyTorque(parseToBulletVector(torque));
}

void RigidBody::clearForces()
{
	checkNullAndBreak(body);
	body->clearForces();
}

void RigidBody::setGravity(const Vector3& grav)
{
	checkNullAndBreak(body);
	body->setGravity(parseToBulletVector(grav));
}

void RigidBody::setDamping(float damping)
{
	checkNullAndBreak(body);
	body->setDamping(btScalar(damping), body->getAngularDamping());
}

void RigidBody::setAngularDamping(float damping)
{
	checkNullAndBreak(body);
	body->setDamping(body->getLinearDamping(), btScalar(damping));
}

void RigidBody::setAngularVelocity(const Vector3& angVel)
{
	checkNullAndBreak(body);
	body->setAngularVelocity(parseToBulletVector(angVel));
}

void RigidBody::setLinearVelocity(const Vector3& vel)
{
	checkNullAndBreak(body);
	body->setLinearVelocity(parseToBulletVector(vel));
}

void RigidBody::setFriction(float friction)
{
	checkNullAndBreak(body);
	body->setFriction(friction);
}

void RigidBody::setRestitution(float restitution)
{
	checkNullAndBreak(body);
	body->setRestitution(restitution);
}

// 1 to allow movement 0 to add a constraint
void RigidBody::setMovementConstraints(const Vector3& constraints)
{
	checkNullAndBreak(body);
	body->setLinearFactor(parseToBulletVector(constraints));
}

// 1 to allow rotation 0 to add a constraint
void RigidBody::setRotationConstraints(const Vector3& constraints)
{
	checkNullAndBreak(body);
	body->setAngularFactor(parseToBulletVector(constraints));
}

void RigidBody::setTrigger(bool trigger)
{
	checkNullAndBreak(body);
	int flag = 0;
	if (isTrigger() && !trigger) flag = -body->CF_NO_CONTACT_RESPONSE;
	else if (!isTrigger() && trigger) flag = body->CF_NO_CONTACT_RESPONSE;
	body->setCollisionFlags(body->getCollisionFlags() + flag);
}

void RigidBody::setKinematic(bool kinematic)
{
	checkNullAndBreak(body);
	int flag = 0;
	if (isKinematic() && !kinematic) flag = -body->CF_KINEMATIC_OBJECT;
	else if (!isKinematic() && kinematic) flag = body->CF_KINEMATIC_OBJECT;
	body->setCollisionFlags(body->getCollisionFlags() + flag);
}

void RigidBody::setStatic(bool stat)
{
	checkNullAndBreak(body);
	int flag = 0;
	if (isStatic() && !stat) flag = -body->CF_STATIC_OBJECT;
	else if (!isStatic() && stat && body->getMass() > 0) flag = body->CF_STATIC_OBJECT;
	body->setCollisionFlags(body->getCollisionFlags() + flag);
}

void RigidBody::setActive(bool active)
{
	Component::setActive(active);

	checkNullAndBreak(body);
	int state = (active) ? ((!isStatic()) ? DISABLE_DEACTIVATION : ACTIVE_TAG) : DISABLE_SIMULATION;
	body->forceActivationState(state);
}

// Multiplies original rigidBody scale with the input vector
void RigidBody::multiplyScale(const Vector3& scale)
{
	checkNullAndBreak(body);
	body->getCollisionShape()->setLocalScaling(parseToBulletVector(scale));
}

void RigidBody::updateTransform()
{
	checkNullAndBreak(body);
	if (!isKinematic()) {
		setKinematic(true);
		btMotionState* motionState = body->getMotionState();
		checkNullAndBreak(motionState);
		motionState->getWorldTransform(body->getWorldTransform());
		setKinematic(false);
	}
}

void RigidBody::disableDeactivation()
{
	checkNullAndBreak(body);
	body->forceActivationState(DISABLE_DEACTIVATION);//body->setActivationState(DISABLE_DEACTIVATION);
}

bool RigidBody::isTrigger() const
{
	checkNullAndBreak(body, false);
	return (body->getCollisionFlags() & body->CF_NO_CONTACT_RESPONSE) != 0;
}

bool RigidBody::isKinematic() const
{
	checkNullAndBreak(body, false);
	return body->isKinematicObject();
}

bool RigidBody::isStatic() const
{
	checkNullAndBreak(body, false);
	return body->isStaticObject();
}

float RigidBody::getLinearDamping() const
{
	checkNullAndBreak(body, 0.0f);
	return body->getLinearDamping();
}

float RigidBody::getFriction() const
{
	checkNullAndBreak(body, 0.0f);
	return body->getFriction();
}

float RigidBody::getRestitution() const
{
	checkNullAndBreak(body, 0.0f);
	return body->getRestitution();
}

Vector3 RigidBody::getGravity() const
{
	checkNullAndBreak(body, Vector3::ZERO);
	return parseFromBulletVector(body->getGravity());
}

Vector3 RigidBody::getAngularVelocity() const
{
	checkNullAndBreak(body, Vector3::ZERO);
	return parseFromBulletVector(body->getAngularVelocity());
}

Vector3 RigidBody::getLinearVelocity() const
{
	checkNullAndBreak(body, Vector3::ZERO);
	return parseFromBulletVector(body->getLinearVelocity());
}

Vector3 RigidBody::getTotalForce() const
{
	checkNullAndBreak(body, Vector3::ZERO);
	return parseFromBulletVector(body->getTotalForce());
}

Vector3 RigidBody::getTotalTorque() const
{
	checkNullAndBreak(body, Vector3::ZERO);
	return parseFromBulletVector(body->getTotalTorque());
}

Vector3 RigidBody::getOrientation() const
{
	checkNullAndBreak(body, Vector3::ZERO);
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

void RigidBody::initPresets()
{
	colPresets["IgnoreRaycast"] = IGNORE_RAYCAST;
	colPresets["ColGroup1"] = COL_GROUP_1;
	colPresets["ColGroup2"] = COL_GROUP_2;
	colPresets["ColGroup3"] = COL_GROUP_3;
	colPresets["ColGroup4"] = COL_GROUP_4;
	colPresets["ColGroup5"] = COL_GROUP_5;
	colPresets["ColGroup6"] = COL_GROUP_6;
	colPresets["ColGroup7"] = COL_GROUP_7;
	colPresets["ColGroup8"] = COL_GROUP_8;
	colPresets["ColGroup9"] = COL_GROUP_9;
	colPresets["ColGroup10"] = COL_GROUP_10;
	colPresets["ColGroup11"] = COL_GROUP_11;
	colPresets["ColGroup12"] = COL_GROUP_12;
	colPresets["ColGroup13"] = COL_GROUP_13;
	colPresets["ColGroup14"] = COL_GROUP_14;
	colPresets["Default"] = DEFAULT;
	colPresets["None"] = NONE;
	colPresets["All"] = ALL;
}

void RigidBody::handleData(ComponentData* data)
{
	checkNullAndBreak(data);
	PhysicsSystem* physicsSystem = PhysicsSystem::GetInstance();
	checkNullAndBreak(physicsSystem);

	float mass = 1.0, damping = 0.0, friction = 0.0, restitution = 0.0, angularDamping = 0.0;
	RB_Shape shape = BOX_RB_SHAPE;
	Vector3 off = Vector3(), dim = Vector3(1, 1, 1), gravity = physicsSystem->getWorldGravity(), movConstraints = { 1,1,1 }, rotConstraints = { 1,1,1 };
	bool isTrigger = false, kinematic = false;
	uint16_t myGroup = DEFAULT, collidesWith = ALL;

	for (auto prop : data->getProperties()) {
		std::stringstream ss(prop.second);

		if (prop.first == "shape") {
			if (prop.second == "Box")
				shape = BOX_RB_SHAPE;
			else if (prop.second == "Sphere")
				shape = SPHERE_RB_SHAPE;
			else if (prop.second == "Capsule")
				shape = CAPSULE_RB_SHAPE;
			else if (prop.second == "Cylinder")
				shape = CYLINDER_RB_SHAPE;
			else if (prop.second == "Cone")
				shape = CONE_RB_SHAPE;
			else
				LOG("RIGIDBODY: %s not valid rigidbody shape type\n", prop.second.c_str());
		}
		else if (prop.first == "mass") {
			setFloat(mass);
		}
		else if (prop.first == "friction") {
			setFloat(friction);
		}
		else if (prop.first == "restitution") {
			setFloat(restitution);
		}
		else if (prop.first == "damping") {
			setFloat(damping);
		}
		else if (prop.first == "angularDamping") {
			setFloat(angularDamping);
		}
		else if (prop.first == "offset") {
			setVector3(off);
		}
		else if (prop.first == "scale") {
			setVector3(dim);
		}
		else if (prop.first == "trigger") {
			setBool(isTrigger);
		}
		else if (prop.first == "kinematic") {
			setBool(kinematic);
		}
		else if (prop.first == "gravity") {
			setVector3(gravity);
		}
		else if (prop.first == "movementConstraints") {
			setVector3(movConstraints);
		}
		else if (prop.first == "rotationConstraints") {
			setVector3(rotConstraints);
		}
		else if (prop.first == "collisionGroup") {
			auto it = (colPresets.find(prop.second));
			if (it != colPresets.end())
				myGroup = (*it).second;
			else
				LOG_ERROR("RIGIDBODY", "Wrong value for property %s", prop.first.c_str());
		}
		else if (prop.first == "collidesWith") {
			auto it = (colPresets.find(prop.second));
			if (it != colPresets.end())
				collidesWith = (*it).second;
			else
				LOG_ERROR("RIGIDBODY", "Wrong value for property %s", prop.first.c_str());
		}
		else
			LOG_ERROR("RIGIDBODY", "Property %s does not exist", prop.first.c_str());
	}
	setRigidBody(mass, shape, off, dim, myGroup, collidesWith);

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