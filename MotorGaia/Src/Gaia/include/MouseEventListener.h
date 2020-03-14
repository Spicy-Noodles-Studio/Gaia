#pragma once
#ifndef MOUSE_EVENT_LISTENER_H
#define MOUSE_EVENT_LISTENER_H

#include "EventListener.h"
#include <functional>

class MouseEventListener : public EventListener
{
public:
	MouseEventListener();
	virtual ~MouseEventListener();

private:
	virtual void processEvent(const SDL_Event& event);

private:
	std::function<void(int x, int y)> mouseMotion;
	std::function<void()> mouseLeftButtonDown;
	std::function<void()> mouseRightButtonDown;
	std::function<void()> mouseMiddleButtonDown;
	std::function<void()> mouseLeftButtonUp;
	std::function<void()> mouseRightButtonUp;
	std::function<void()> mouseMiddleButtonUp;
	std::function<void(int value)> mouseWheelScrollY;
	std::function<void(int value)> mouseWheelScrollX;

protected:

	/* Set callback to be called when mouse is moving */
	void onMouseMotion(std::function<void(int x, int y)> callback);

	/* Set callback to be called when mouse left button is down */
	void onMouseLeftButtonDown(std::function<void()> callback);

	/* Set callback to be called when mouse right button is down */
	void onMouseRightButtonDown(std::function<void()> callback);

	/* Set callback to be called when mouse middle button is down */
	void onMouseMiddleButtonDown(std::function<void()> callback);

	/* Set callback to be called when mouse left button is up */
	void onMouseLeftButtonUp(std::function<void()> callback);

	/* Set callback to be called when mouse right button is up */
	void onMouseRightButtonUp(std::function<void()> callback);

	/* Set callback to be called when mouse middle button is up */
	void onMouseMiddleButtonUp(std::function<void()> callback);

	/* Set callback to be called when mouse wheel is scrolling its Y-axis */
	void onMouseWheelScrollY(std::function<void(int value)> callback);

	/* Set callback to be called when mouse wheel is scrolling its X-axis */
	void onMouseWheelScrollX(std::function<void(int value)> callback);
};

#endif 
