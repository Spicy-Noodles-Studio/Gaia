#pragma once
#ifndef EVENT_SYSTEM_H
#define EVENT_SYSTEM_H

#include <vector>
#include "EventListener.h"

class EventSystem
{
	friend class EventListener;
public:
	EventSystem();
	~EventSystem();

	void init();
	void update();
	void close();

private:
	static void addListener(EventListener* listener);
	static void removeListener(EventListener* listener);

private:
	static std::vector<EventListener*> listeners;
	
};
#endif
