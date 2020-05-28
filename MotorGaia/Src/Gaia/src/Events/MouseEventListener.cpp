#include "MouseEventListener.h"


MouseEventListener::MouseEventListener()
{
}

MouseEventListener::~MouseEventListener()
{
}

void MouseEventListener::processEvent(const SDL_Event& event)
{
	switch (event.type)
	{
	case SDL_MOUSEMOTION:
		if (mouseMotion) mouseMotion(event.motion.x, event.motion.y);
		break;
	case SDL_MOUSEBUTTONDOWN:
		if (event.button.button == SDL_BUTTON_LEFT) {
			if (mouseLeftButtonDown) mouseLeftButtonDown();		}
		else if (event.button.button == SDL_BUTTON_RIGHT) {
			if (mouseRightButtonDown) mouseRightButtonDown();
		}
		else if (event.button.button == SDL_BUTTON_MIDDLE) {
			if (mouseMiddleButtonDown) mouseMiddleButtonDown();
		}
		break;
	case SDL_MOUSEBUTTONUP:
		if (event.button.button == SDL_BUTTON_LEFT) {
			if (mouseLeftButtonUp) mouseLeftButtonUp();
		}
		else if (event.button.button == SDL_BUTTON_RIGHT) {
			if (mouseRightButtonUp) mouseRightButtonUp();
		}
		else if (event.button.button == SDL_BUTTON_MIDDLE) {
			if (mouseMiddleButtonUp) mouseMiddleButtonUp();
		}
		break;
	case SDL_MOUSEWHEEL:
		if (event.wheel.y != 0 && mouseWheelScrollY) mouseWheelScrollY(event.wheel.direction == SDL_MOUSEWHEEL_NORMAL ? event.wheel.y : -event.wheel.y);
		if (event.wheel.x != 0 && mouseWheelScrollX) mouseWheelScrollX(event.wheel.direction == SDL_MOUSEWHEEL_NORMAL ? event.wheel.x : -event.wheel.x);
		break;
	default:
		break;
	}
}

void MouseEventListener::onMouseMotion(std::function<void(int x, int y)> callback)
{
	mouseMotion = callback;
}

void MouseEventListener::onMouseLeftButtonDown(std::function<void()> callback)
{
	mouseLeftButtonDown = callback;
}

void MouseEventListener::onMouseRightButtonDown(std::function<void()> callback)
{
	mouseRightButtonDown = callback;
}

void MouseEventListener::onMouseMiddleButtonDown(std::function<void()> callback)
{
	mouseMiddleButtonDown = callback;
}

void MouseEventListener::onMouseLeftButtonUp(std::function<void()> callback)
{
	mouseLeftButtonUp = callback;
}

void MouseEventListener::onMouseRightButtonUp(std::function<void()> callback)
{
	mouseRightButtonUp = callback;
}

void MouseEventListener::onMouseMiddleButtonUp(std::function<void()> callback)
{
	mouseMiddleButtonUp = callback;
}

void MouseEventListener::onMouseWheelScrollY(std::function<void(int value)> callback)
{
	mouseWheelScrollY = callback;
}

void MouseEventListener::onMouseWheelScrollX(std::function<void(int value)> callback)
{
	mouseWheelScrollX = callback;
}
