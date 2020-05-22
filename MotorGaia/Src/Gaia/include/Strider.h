#pragma once
#ifndef STRIDER_H
#define STRIDER_H

#include "RigidBody.h"

class MeshStrider;

class GAIA_API Strider : public RigidBody
{
private:
	MeshStrider* meshStrider;
	uint16_t myGroup, collidesWith;

public:
	Strider(GameObject* gameObject);
	virtual ~Strider();

	void stride(const std::string& mesh);

protected:
	virtual void handleData(ComponentData* data);
};
#endif
