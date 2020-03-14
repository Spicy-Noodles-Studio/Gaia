#include "KeyboardEventListener.h"


KeyboardEventListener::KeyboardEventListener()
{
}

KeyboardEventListener::~KeyboardEventListener()
{
}

void KeyboardEventListener::processEvent(const SDL_Event& event)
{
	switch (event.type)
	{
	case SDL_KEYDOWN:
		if (keyDown) keyDown(SDL_GetKeyName(event.key.keysym.sym), event.key.keysym.sym);
		if (!event.key.repeat && keyPressed) keyPressed(SDL_GetKeyName(event.key.keysym.sym), event.key.keysym.sym);
		break;
	case SDL_KEYUP:
		if (keyUp) keyUp(SDL_GetKeyName(event.key.keysym.sym), event.key.keysym.sym);
		if (!event.key.repeat && keyReleased) keyReleased(SDL_GetKeyName(event.key.keysym.sym), event.key.keysym.sym);
		break;
	case SDL_TEXTINPUT:
		if (textInput) textInput(std::string(event.text.text));
		break;
	default:
		break;
	}
}

void KeyboardEventListener::onKeyDown(std::function<void(std::string keyName, int key)> callback)
{
	keyDown = callback;
}

void KeyboardEventListener::onKeyUp(std::function<void(std::string keyName, int key)> callback)
{
	keyUp = callback;
}

void KeyboardEventListener::onKeyPressed(std::function<void(std::string keyName, int key)> callback)
{
	keyPressed = callback;
}

void KeyboardEventListener::onKeyReleased(std::function<void(std::string keyName, int key)> callback)
{
	keyReleased = callback;
}

void KeyboardEventListener::onTextInput(std::function<void(std::string text)> callback)
{
	textInput = callback;
}
