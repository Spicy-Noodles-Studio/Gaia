#pragma once
#ifndef USER_COMPONENT_H
#define USER_COMPONENT_H

#include <vector>
#include "Component.h"
#include "Vector3.h"

#include "GameObjectData.h"

class GAIA_API UserComponent : public Component
{
	friend class Scene;
public:
	UserComponent(GameObject* gameObject);
	virtual ~UserComponent();

	virtual void awake();
	virtual void start();
	virtual void preUpdate(float deltaTime);
	virtual void update(float deltaTime);
	virtual void postUpdate(float deltaTime);

	virtual void onCollisionEnter(GameObject* other);
	virtual void onCollisionStay(GameObject* other);
	virtual void onCollisionExit(GameObject* other);

	virtual void onTriggerEnter(GameObject* other);
	virtual void onTriggerStay(GameObject* other);
	virtual void onTriggerExit(GameObject* other);

	virtual void onObjectEnter(GameObject* other);
	virtual void onObjectStay(GameObject* other);
	virtual void onObjectExit(GameObject* other);

	GameObject* instantiate(const std::string& blueprintName, const Vector3& position = Vector3( 0, 0, 0));
	void destroy(GameObject* gameObject);

	// Busca a partir de la referencia de Scene desde el owner un objeto en la misma escena
	// con el nombre indicado
	GameObject* findGameObjectWithName(const std::string& name);

	// Busca a partir de la referencia de Scene desde el owner un objeto en la misma escena
	// con el tag indicado
	std::vector<GameObject*> findGameObjectsWithTag(const std::string& tag);

private:
	bool hasStarted();
	bool isSleeping();
	GameObject* instantiate(const GameObjectData* data);

private:
	bool started;
	bool sleeping;
};

#endif