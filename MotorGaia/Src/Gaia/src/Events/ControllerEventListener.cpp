#include "ControllerEventListener.h"


ControllerEventListener::ControllerEventListener()
{
}

ControllerEventListener::~ControllerEventListener()
{
}

void ControllerEventListener::processEvent(const SDL_Event& event)
{

	switch (event.type)
	{
	case SDL_CONTROLLERAXISMOTION:
		if (event.caxis.axis == SDL_CONTROLLER_AXIS_INVALID) {
			if (controllerAxisInvalid) controllerAxisInvalid(event.caxis.which);
		}
		else if (event.caxis.axis == SDL_CONTROLLER_AXIS_LEFTX){
			if (controllerAxisLeftXMotion) controllerAxisLeftXMotion(event.caxis.which, (double)event.caxis.value / (double)SDL_JOYSTICK_AXIS_MAX);
		}
		else if (event.caxis.axis == SDL_CONTROLLER_AXIS_LEFTY) {
			if (controllerAxisLeftYMotion) controllerAxisLeftYMotion(event.caxis.which, (double)event.caxis.value / (double)SDL_JOYSTICK_AXIS_MAX);
		}
		else if (event.caxis.axis == SDL_CONTROLLER_AXIS_RIGHTX) {
			if (controllerAxisRightXMotion) controllerAxisRightXMotion(event.caxis.which, (double)event.caxis.value / (double)SDL_JOYSTICK_AXIS_MAX);
		}
		else if (event.caxis.axis == SDL_CONTROLLER_AXIS_RIGHTY) {
			if (controllerAxisRightYMotion) controllerAxisRightYMotion(event.caxis.which, (double)event.caxis.value / (double)SDL_JOYSTICK_AXIS_MAX);
		}
		else if (event.caxis.axis == SDL_CONTROLLER_AXIS_TRIGGERLEFT) {
			if (controllerAxisLeftTriggerMotion) controllerAxisLeftTriggerMotion(event.caxis.which, (double)event.caxis.value / (double)SDL_JOYSTICK_AXIS_MAX);
		}
		else if (event.caxis.axis == SDL_CONTROLLER_AXIS_TRIGGERRIGHT) {
			if (controllerAxisRightTriggerMotion) controllerAxisRightTriggerMotion(event.caxis.which, (double)event.caxis.value / (double)SDL_JOYSTICK_AXIS_MAX);
		}
		break;
	case SDL_CONTROLLERBUTTONDOWN:
		if (controllerButtonDown) controllerButtonDown(event.cbutton.which, event.cbutton.button);
		break;
	case SDL_CONTROLLERBUTTONUP:
		if (controllerButtonUp) controllerButtonUp(event.cbutton.which, event.cbutton.button);
		break;
	case SDL_CONTROLLERDEVICEADDED:
		if (controllerDeviceAdded) controllerDeviceAdded(event.cdevice.which);
		break;
	case SDL_CONTROLLERDEVICEREMOVED:
		if (controllerDeviceRemoved) controllerDeviceRemoved(event.cdevice.which);
		break;
	case SDL_CONTROLLERDEVICEREMAPPED:
		if (controllerDeviceRemapped) controllerDeviceRemapped(event.cdevice.which);
		break;
	default:
		break;
	}
	
}

void ControllerEventListener::onControllerAxisInvalid(std::function<void(int index)> callback)
{
	controllerAxisInvalid = callback;
}

void ControllerEventListener::onControllerAxisLeftXMotion(std::function<void(int index, double value)> callback)
{
	controllerAxisLeftXMotion = callback;
}

void ControllerEventListener::onControllerAxisLeftYMotion(std::function<void(int index, double value)> callback)
{
	controllerAxisLeftYMotion = callback;
}

void ControllerEventListener::onControllerAxisRightXMotion(std::function<void(int index, double value)> callback)
{
	controllerAxisRightXMotion = callback;
}

void ControllerEventListener::onControllerAxisRightYMotion(std::function<void(int index, double value)> callback)
{
	controllerAxisRightYMotion = callback;
}

void ControllerEventListener::onControllerAxisLeftTriggerMotion(std::function<void(int index, double value)> callback)
{
	controllerAxisLeftTriggerMotion = callback;
}

void ControllerEventListener::onControllerAxisRightTriggerMotion(std::function<void(int index, double value)> callback)
{
	controllerAxisRightTriggerMotion = callback;
}

void ControllerEventListener::onControllerButtonDown(std::function<void(int index, int button)> callback)
{
	controllerButtonDown = callback;
}

void ControllerEventListener::onControllerButtonUp(std::function<void(int index, int button)> callback)
{
	controllerButtonUp = callback;
}

void ControllerEventListener::onControllerDeviceAdded(std::function<void(int index)> callback)
{
	controllerDeviceAdded = callback;
}

void ControllerEventListener::onControllerDeviceRemoved(std::function<void(int index)> callback)
{
	controllerDeviceRemoved = callback;
}

void ControllerEventListener::onControllerDeviceRemapped(std::function<void(int index)> callback)
{
	controllerDeviceRemapped = callback;
}
