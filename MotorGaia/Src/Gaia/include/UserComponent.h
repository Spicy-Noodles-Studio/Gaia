#pragma once
#ifndef USER_COMPONENT_H
#define USER_COMPONENT_H

#include "Component.h"
//#include "Vector3.h"

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

	void destroy(GameObject* gameObject);
	// void instantiate(const std::string& blueprintName, const Vector3& position = Vector3( 0, 0, 0), const Vector3& rotation = Vector3(0, 0, 0));
	// void instantiate(const GameObject* gameObject, const Vector3& position = Vector3(0, 0, 0), const Vector3& rotation = Vector3(0, 0, 0));

	bool hasStarted();
	bool isSleeping();
	void setSleeping(bool sleep);

private:
	bool started;
	bool sleeping;
};

#endif