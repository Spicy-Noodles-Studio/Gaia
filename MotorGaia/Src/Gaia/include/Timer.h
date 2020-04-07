#pragma once
#ifndef TIME_H
#define TIME_H

#include "Singleton.h"
#include <ctime>
#include <chrono>

class GAIA_API Timer : public Singleton<Timer>
{
private:
	float deltaTime;
	float timeScale;
	std::chrono::steady_clock::time_point timeSinceLast;

public:
	Timer();
	~Timer();

	/* Initialize timer */
	void init();

	/* Close and destroys the singleton */
	void close();

	/* Updates the delta since the last time this function was called */
	void update();

	/* Get the deltaTime in seconds */
	float getDeltaTime() const;

	/* Set the time scale */
	void setTimeScale(float scale);
};

#endif
