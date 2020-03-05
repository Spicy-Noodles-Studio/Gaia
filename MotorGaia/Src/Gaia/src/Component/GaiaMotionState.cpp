#include "GaiaMotionState.h"
#include "Transform.h"
#include "PhysicsSystem.h"

GaiaMotionState::GaiaMotionState(Transform* transform) : transform(transform)
{
	
}

GaiaMotionState::~GaiaMotionState()
{
}

Transform* GaiaMotionState::getTransform() const
{
	return transform;
}

void GaiaMotionState::getWorldTransform(btTransform& worldTrans) const
{
	worldTrans = PhysicsSystem::GetInstance()->parseToBulletTransform(transform);
}

void GaiaMotionState::setWorldTransform(const btTransform& worldTrans)
{
	btQuaternion rot = worldTrans.getRotation();
	transform->setOrientation({ rot.w(), rot.x(), rot.y(), rot.z() });
	btVector3 pos = worldTrans.getOrigin();
	transform->setPosition(pos.x(), pos.y() + 5, pos.z() - 5);
}
