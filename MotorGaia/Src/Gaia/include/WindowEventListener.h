#pragma once
#ifndef WINDOW_EVENT_LISTENER_H
#define WINDOW_EVENT_LISTENER_H

#include "EventListener.h"
#include <functional>

class WindowEventListener : public EventListener
{
public:
	WindowEventListener();
	virtual ~WindowEventListener();

private:
	virtual void processEvent(const SDL_Event& event);

private:
	std::function<void()> windowShown;
	std::function<void()> windowHidden;
	std::function<void()> windowExposed;
	std::function<void(int x, int y)> windowMoved;
	std::function<void(unsigned int width, unsigned int height)> windowResized;
	std::function<void(unsigned int width, unsigned int height)> windowSizeChanged;
	std::function<void()> windowMinimized;
	std::function<void()> windowMaximized;
	std::function<void()> windowRestored;
	std::function<void()> windowMouseEnter;
	std::function<void()> windowMouseLeave;
	std::function<void()> windowKeyboardFocusGained;
	std::function<void()> windowKeyboardFocusLost;
	std::function<void()> windowClose;

protected:
	/* Set callback to be called when window is shown */
	void onWindowShown(std::function<void()> callback);

	/* Set callback to be called when window is hidden */
	void onWindowHidden(std::function<void()> callback);

	/* Set callback to be called when window is exposed */
	void onWindowExposed(std::function<void()> callback);

	/* Set callback to be called when window is moved */
	void onWindowMoved(std::function<void(int x, int y)> callback);

	/* Set callback to be called when window is resized */
	void onWindowResized(std::function<void(unsigned int width, unsigned int height)> callback);

	/* Set callback to be called when window size is changed */
	void onWindowSizeChanged(std::function<void(unsigned int width, unsigned int height)> callback);

	/* Set callback to be called when window is minimized */
	void onWindowMinimized(std::function<void()> callback);

	/* Set callback to be called when window is maximized */
	void onWindowMaximized(std::function<void()> callback);

	/* Set callback to be called when window is restored */
	void onWindowRestored(std::function<void()> callback);

	/* Set callback to be called when mouse enters the window */
	void onWindowMouseEnter(std::function<void()> callback);

	/* Set callback to be called when mouse leaves the window */
	void onWindowMouseLeave(std::function<void()> callback);

	/* Set callback to be called when window gains keyboard focus */
	void onWindowKeyboardFocusGained(std::function<void()> callback);

	/* Set callback to be called when window loses keyboard focus */
	void onWindowKeyboardFocusLost(std::function<void()> callback);

	/* Set callback to be called when window close button is pressed */
	void onWindowClose(std::function<void()> callback);
};
#endif