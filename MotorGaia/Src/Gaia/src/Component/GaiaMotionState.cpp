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
	worldTrans.setOrigin(worldTrans.getOrigin() + offset);
}

void GaiaMotionState::setWorldTransform(const btTransform& worldTrans)
{
	btScalar x, y, z;
	worldTrans.getRotation().getEulerZYX(z, y, x);
	transform->setWorldRotation({ x / SIMD_RADS_PER_DEG, y / SIMD_RADS_PER_DEG, z / SIMD_RADS_PER_DEG });
	btVector3 pos = worldTrans.getOrigin();
	transform->setWorldPosition({ pos.x() - offset.x, pos.y() - offset.y, pos.z() - offset.z });
}
