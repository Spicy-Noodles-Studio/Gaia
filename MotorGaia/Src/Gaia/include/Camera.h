#pragma once
#ifndef CAMERA_H
#define CAMERA_H

#include "GaiaComponent.h"
#include "Vector3.h"
#include "Quaternion.h"

namespace Ogre {
	class Camera;
}

class GAIA_API Camera : public GaiaComponent
{
	friend class WindowManager;
public:
	enum class SpaceReference { LocalSpace, WorldSpace, ParentSpace };

private:
	Ogre::Camera* camera;
	bool isMainCamera;

public:
	Camera(GameObject* gameObject);
	virtual ~Camera();
	
	void lookAt(const Vector3& position, SpaceReference space);

	Vector3 getDirection() const;
	Quaternion getOrientation() const;
	void setDirection(const Vector3& direction);
	void setOrientation(const Quaternion& orientation);

	Vector3 worldToScreen(const Vector3& worldPoint);
	Vector3 worldToScreenPixel(const Vector3& worldPoint);

	void setClipDistances(double near, double far);

protected:
	virtual void handleData(ComponentData* data);

private:
	Ogre::Camera* getCamera() const;
};

#endif