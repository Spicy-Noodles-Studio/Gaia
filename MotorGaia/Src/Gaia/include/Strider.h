#pragma once

#ifndef STRIDER_H
#define STRIDER_H

#include "RigidBody.h"

class MeshStrider;

class Strider : public RigidBody
{
private:
	MeshStrider* meshStrider;

public:
	Strider(GameObject* gameObject);
	~Strider();

	virtual void handleData(ComponentData* data);
	void stride(const std::string& mesh);
};
#endif
