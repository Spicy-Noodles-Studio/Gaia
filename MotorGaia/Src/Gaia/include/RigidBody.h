#pragma once
#ifndef RIGID_BODY_H
#define RIGID_BODY_H
#include "GaiaComponent.h"
#include "PhysicsSystem.h"

class GaiaMotionState;

enum ImpulseMode
{
	IMPULSE, PUSH, TORQUE, TORQUE_TURN
};

class RigidBody : public GaiaComponent
{
private:
	btRigidBody* body = nullptr;
	static Vector3 btScaleConversion;
	GaiaMotionState* motionState;
	bool trigger;

	// Turns a Gaia Vector3 into a Bullet Physics Vector3
	const btVector3 parseToBulletVector(const Vector3& v) const;
	const Vector3 parseFromBulletVector(const btVector3& v) const;

public:
	RigidBody(GameObject* gameObject);
	~RigidBody();

	void setRigidBody(float mass, RB_Shape shape, const Vector3& offset = { 0.0f, 0.0f, 0.0f }, const Vector3& dim = { 1,1,1 }, bool isTrigger = false, uint16_t myGroup = DEFAULT, uint16_t collidesWith = ALL);
	void handleData(ComponentData* data);
	bool isTrigger() const;

	void addForce(const Vector3 &force, Vector3 rel_pos = { 0.0f, 0.0f, 0.0f });
	void addImpulse(const Vector3 &impulse, ImpulseMode mode = IMPULSE, Vector3 rel_pos = { 0.0f, 0.0f, 0.0f });
	void addTorque(const Vector3 &torque);

	void setGravity(const Vector3 &grav);
	void setDamping(float damping);
	void setAngularDamping(float damping);
	void setAngularVelocity(const Vector3& ang_vel);
	void setLinearVelocity(const Vector3& vel);
	void setFriction(float friction);
	void setRestitution(float restitution);
	
	const Vector3 &getGravity() const;
	float getLinearDamping();
	bool isActive();
	const Vector3 &getAngularVelocity() const;
	float getFriction();
	const Vector3 &getLinearVelocity() const;
	float getRestitution();
	const Vector3 &getTotalForce() const;
	const Vector3 &getTotalTorque() const;
	const Ogre::Quaternion&getOrientation() const;
};

#endif
