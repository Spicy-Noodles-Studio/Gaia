#include "gTime.h"
#include <OgreRoot.h>

gTime::gTime()
{
}


gTime::~gTime()
{
}

void gTime::setup()
{
	deltaTime = 0.0f;
	timeSinceStart = 0.0f;
	Ogre::Root::getSingleton().addFrameListener(this);
}

float gTime::getDeltaTime() const
{
	return deltaTime;
}

void gTime::resetTime()
{
	timeSinceStart = 0.0f;
}


bool gTime::frameStarted(const Ogre::FrameEvent& evt)
{
	deltaTime = evt.timeSinceLastFrame;
	timeSinceStart += deltaTime;
	return true;
}