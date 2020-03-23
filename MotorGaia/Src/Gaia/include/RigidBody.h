#pragma once
#ifndef RIGID_BODY_H
#define RIGID_BODY_H

#include "PhysicsUtils.h"
#include "Vector3.h"
#include "GaiaComponent.h"

class GaiaMotionState;
class btRigidBody;
class btVector3;
class GameObject;
class ComponentData;

class GAIA_API RigidBody : public GaiaComponent
{
protected:
	btRigidBody* body = nullptr;
	GaiaMotionState* motionState = nullptr;

	// Turns a Gaia Vector3 into a Bullet Physics Vector3
	const btVector3 parseToBulletVector(const Vector3& v) const;
	const Vector3 parseFromBulletVector(const btVector3& v) const;

public:
	RigidBody(GameObject* gameObject);
	~RigidBody();

	void setRigidBody(float mass, RB_Shape shape, const Vector3& offset = { 0.0f, 0.0f, 0.0f }, const Vector3& dim = { 1,1,1 }, uint16_t myGroup = DEFAULT, uint16_t collidesWith = ALL);
	void handleData(ComponentData* data);

	void addForce(const Vector3 &force, Vector3 relPos = { 0.0f, 0.0f, 0.0f });
	void addImpulse(const Vector3 &impulse, ImpulseMode mode = IMPULSE, Vector3 relPos = { 0.0f, 0.0f, 0.0f });
	void addTorque(const Vector3 &torque);

	void setGravity(const Vector3 &grav);
	void setDamping(float damping);
	void setAngularDamping(float damping);
	void setAngularVelocity(const Vector3& angVel);
	void setLinearVelocity(const Vector3& vel);
	void setFriction(float friction);
	void setRestitution(float restitution);
	void setMovementConstraints(const Vector3& constraints);
	void setRotationConstraints(const Vector3& constraints);

	void setTrigger(bool trigger);
	void setKinematic(bool kinematic);
	void setStatic(bool stat);

	virtual void setActive(bool active);
	void multiplyScale(const Vector3& scale);
	void updateTransform();

	void disableDeactivation();

	bool isTrigger() const;
	bool isKinematic() const;
	bool isStatic() const;

	float getLinearDamping() const;
	float getFriction() const;
	float getRestitution() const;
	const Vector3& getGravity() const;
	const Vector3& getAngularVelocity() const;
	const Vector3& getLinearVelocity() const;
	const Vector3& getTotalForce() const;
	const Vector3& getTotalTorque() const;
	const Vector3& getOrientation() const;
};

#endif
