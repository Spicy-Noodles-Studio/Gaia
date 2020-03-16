#include "EventSystem.h"
#include "DebugUtils.h"

std::vector<EventListener*> EventSystem::listeners;

EventSystem::EventSystem()
{

}

EventSystem::~EventSystem()
{

}

void EventSystem::init()
{
	if (SDL_Init(SDL_INIT_EVENTS | SDL_INIT_JOYSTICK | SDL_INIT_HAPTIC | SDL_INIT_GAMECONTROLLER) != 0) {
		LOG_ERROR("EVENT SYSTEM","system cannot be initialized");
	}
}

void EventSystem::update()
{
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		for (EventListener* listener : listeners)
			listener->processEvent(event);
	}
}

void EventSystem::close()
{
	SDL_Quit();
}

void EventSystem::addListener(EventListener* listener)
{
	listeners.push_back(listener);
}

void EventSystem::removeListener(EventListener* listener)
{
	auto it = std::find(listeners.begin(), listeners.end(), listener);
	if (it == listeners.end())
		return;
	listeners.erase(it);
}
