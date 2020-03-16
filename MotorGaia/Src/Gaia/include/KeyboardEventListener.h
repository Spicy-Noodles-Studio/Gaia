#pragma once
#ifndef KEYBOARD_EVENT_LISTENER_H
#define KEYBOARD_EVENT_LISTENER_H

#include "EventListener.h"
#include <functional>
#include <string>

class KeyboardEventListener : public EventListener
{
public:
	KeyboardEventListener();
	virtual ~KeyboardEventListener();

private:
	virtual void processEvent(const SDL_Event& event);

private:
	std::function<void(std::string keyName, int key)> keyDown;
	std::function<void(std::string keyName, int key)> keyUp;
	std::function<void(std::string keyName, int key)> keyPressed;
	std::function<void(std::string keyName, int key)> keyReleased;
	std::function<void(std::string text)> textInput;

protected:

	/* Set callback to be called when key is down */
	void onKeyDown(std::function<void(std::string keyName, int key)> callback);

	/* Set callback to be called when key is up */
	void onKeyUp(std::function<void(std::string keyName, int key)> callback);

	/* Set callback to be called when key is pressed */
	void onKeyPressed(std::function<void(std::string keyName, int key)> callback);

	/* Set callback to be called when key is released */
	void onKeyReleased(std::function<void(std::string keyName, int key)> callback);

	/* Set callback to be called when a text is input */
	/* text: gives the input text (utf-8 format) */
	void onTextInput(std::function<void(std::string text)> callback);

};

#endif 
