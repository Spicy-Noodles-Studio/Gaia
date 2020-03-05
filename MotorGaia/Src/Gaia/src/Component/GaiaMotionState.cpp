#include "GaiaMotionState.h"
#include "Transform.h"
#include "PhysicsSystem.h"

GaiaMotionState::GaiaMotionState(Transform* transform, const Vector3& off) : transform(transform)
{
	offset = off;
}

GaiaMotionState::~GaiaMotionState()
{
}

Transform* GaiaMotionState::getTransform() const
{
	return transform;
}

void GaiaMotionState::setOffset(const Vector3& off)
{
	offset = off;
}

void GaiaMotionState::getWorldTransform(btTransform& worldTrans) const
{
	worldTrans = PhysicsSystem::GetInstance()->parseToBulletTransform(transform);
	worldTrans.setOrigin(worldTrans.getOrigin()+offset);
}

void GaiaMotionState::setWorldTransform(const btTransform& worldTrans)
{
	btQuaternion rot = worldTrans.getRotation();
	transform->setOrientation({ rot.w(), rot.x(), rot.y(), rot.z() });
	btVector3 pos = worldTrans.getOrigin();
	transform->setPosition(pos.x()-offset.x, pos.y()-offset.y, pos.z()-offset.z);
}
