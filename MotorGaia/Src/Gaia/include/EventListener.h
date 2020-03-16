#pragma once
#ifndef EVENT_LISTENER_H
#define EVENT_LISTENER_H

#include <SDL2\SDL.h>

class EventListener
{
	friend class EventSystem;
public:
	EventListener();
	virtual ~EventListener();

private:
	virtual void processEvent(const SDL_Event& event) = 0;

};
#endif
