#pragma once

#ifndef GAIA_MOTION_STATE_H
#define GAIA_MOTION_STATE_H

#include "btBulletDynamicsCommon.h"
class Transform;

// This class extends the Motion State from Bullet Physics Library
// It uses Gaia's Transform Component and update its position when 
// Bullet Physics calls the method "setWorldTransform"
class GaiaMotionState : public btMotionState
{
protected:
	Transform* transform;
public:
	GaiaMotionState(Transform* transform);
	~GaiaMotionState();

	Transform* getTransform() const;

	// Methods from bullet physics

	virtual void getWorldTransform(btTransform& worldTrans) const;
	virtual void setWorldTransform(const btTransform& worldTrans);
};


#endif