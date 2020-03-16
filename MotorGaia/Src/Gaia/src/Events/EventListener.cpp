#include "EventListener.h"
#include "EventSystem.h"

EventListener::EventListener()
{
	EventSystem::addListener(this);
}

EventListener::~EventListener()
{
	EventSystem::removeListener(this);
}
