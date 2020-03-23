#pragma once

#ifndef GAIA_MOTION_STATE_H
#define GAIA_MOTION_STATE_H

#include "btBulletDynamicsCommon.h"
#include "Vector3.h"

class Transform;

// This class extends the Motion State from Bullet Physics Library
// It uses Gaia's Transform Component and update its position when 
// Bullet Physics calls the method "setWorldTransform"
class GaiaMotionState : public btMotionState
{
protected:
	Transform* transform;
	Vector3 offset = Vector3();
public:
	GaiaMotionState(Transform* transform, const Vector3& offset = { 0,0,0 });
	~GaiaMotionState();

	Transform* getTransform() const;

	void setOffset(const Vector3& offset);
	// Methods from bullet physics

	virtual void getWorldTransform(btTransform& worldTrans) const;
	virtual void setWorldTransform(const btTransform& worldTrans);
};


#endif