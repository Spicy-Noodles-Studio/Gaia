#pragma once
#include "Component.h"


class UserComponent : public Component
{
public:
	UserComponent(GameObject* gameObject);
	~UserComponent();

	virtual void awake();
	virtual void start();
	virtual void preUpdate(float deltaTime);
	virtual void update(float deltaTime);
	virtual void postUpdate(float deltaTime);

	virtual void onCollisionEnter(GameObject* other);
	virtual void onCollisionStay(GameObject* other);
	virtual void onCollisionExit(GameObject* other);
};