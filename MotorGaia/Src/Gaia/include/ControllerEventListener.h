#pragma once
#ifndef CONTROLLER_EVENT_LISTENER_H
#define CONTROLLER_EVENT_LISTENER_H

#include "EventListener.h"
#include <functional>

class ControllerEventListener : public EventListener
{
public:
	ControllerEventListener();
	~ControllerEventListener();

private:
	virtual void processEvent(const SDL_Event& event);

private:
	std::function<void(int index)> controllerAxisInvalid;
	std::function<void(int index, double value)> controllerAxisLeftXMotion;
	std::function<void(int index, double value)> controllerAxisLeftYMotion;
	std::function<void(int index, double value)> controllerAxisRightXMotion;
	std::function<void(int index, double value)> controllerAxisRightYMotion;
	std::function<void(int index, double value)> controllerAxisLeftTriggerMotion;
	std::function<void(int index, double value)> controllerAxisRightTriggerMotion;

	std::function<void(int index, int button)> controllerButtonDown;
	std::function<void(int index, int button)> controllerButtonUp;

	std::function<void(int index)> controllerDeviceAdded;
	std::function<void(int index)> controllerDeviceRemoved;
	std::function<void(int index)> controllerDeviceRemapped;

protected:

	/* Set callback to be called when invalid axis is moved */
	void onControllerAxisInvalid(std::function<void(int index)> callback);

	/* Set callback to be called when left X axis is moved */
	void onControllerAxisLeftXMotion(std::function<void(int index, double value)> callback);

	/* Set callback to be called when left Y axis is moved */
	void onControllerAxisLeftYMotion(std::function<void(int index, double value)> callback);

	/* Set callback to be called when right X axis is moved */
	void onControllerAxisRightXMotion(std::function<void(int index, double value)> callback);

	/* Set callback to be called when right Y axis is moved */
	void onControllerAxisRightYMotion(std::function<void(int index, double value)> callback);

	/* Set callback to be called when left trigger is moved */
	void onControllerAxisLeftTriggerMotion(std::function<void(int index, double value)> callback);

	/* Set callback to be called when right trigger is moved */
	void onControllerAxisRightTriggerMotion(std::function<void(int index, double value)> callback);

	/* Set callback to be called when button down */
	void onControllerButtonDown(std::function<void(int index, int button)> callback);

	/* Set callback to be called when button up */
	void onControllerButtonUp(std::function<void(int index, int button)> callback);

	/* Set callback to be called when controller device is added */
	void onControllerDeviceAdded(std::function<void(int index)> callback);

	/* Set callback to be called when controller device is removed */
	void onControllerDeviceRemoved(std::function<void(int index)> callback);

	/* Set callback to be called when controller device is remapped */
	void onControllerDeviceRemapped(std::function<void(int index)> callback);

};

#endif