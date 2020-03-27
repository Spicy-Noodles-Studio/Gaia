#pragma once
#ifndef CAMERA_DEBUG_CONTROL_H
#define CAMERA_DEBUG_CONTROL_H

#include "UserComponent.h"
#include "Camera.h"

class CameraDebugControl : public UserComponent
{
private:
	std::pair<int, int> mousePos;
	Camera* camera;
public:
	CameraDebugControl(GameObject* gameObject);
	~CameraDebugControl();

	virtual void start();
	virtual void update(float deltaTime);

};


#endif