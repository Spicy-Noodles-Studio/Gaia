#pragma once
#ifndef USER_COMPONENT_H
#define USER_COMPONENT_H

#include "Component.h"
#include <vector>
//#include "Vector3.h"

class UserComponent : public Component
{
	friend class Scene;
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

	// Busca a partir de la referencia de Scene desde el owner un objeto en la misma escena
	// con el nombre indicado
	GameObject* findGameObjectWithName(const std::string& name);

	// Busca a partir de la referencia de Scene desde el owner un objeto en la misma escena
	// con el tag indicado
	std::vector<GameObject*> findGameObjectsWithTag(const std::string& tag);

private:
	bool hasStarted();
	bool isSleeping();
	void setSleeping(bool sleep);

private:
	bool started;
	bool sleeping;
};

#endif