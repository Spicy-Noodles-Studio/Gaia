#include "Timer.h"
#include "DebugUtils.h"


Timer::Timer() : deltaTime(0.0f), timeScale(1.0f)
{

}

Timer::~Timer()
{

}

void Timer::init()
{
	deltaTime = 0.0f;
	timeScale = 1.0f;
	timeSinceLast = std::chrono::steady_clock::now();
}

void Timer::close()
{
	destroy();
}

void Timer::update()
{
	std::chrono::steady_clock::time_point current = std::chrono::steady_clock::now();
	std::chrono::duration<float> elapsed = std::chrono::duration_cast<std::chrono::duration<float>>(current - timeSinceLast);
	deltaTime = elapsed.count();
	timeSinceLast = current;
	//LOG("Time Elapsed: %f | FPS : %i", deltaTime, (int)(1.0f / deltaTime));
}

float Timer::getDeltaTime() const
{
	return deltaTime * timeScale;
}

void Timer::setTimeScale(float scale)
{
	timeScale = scale;
}
