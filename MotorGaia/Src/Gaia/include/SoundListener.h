#pragma once
#ifndef SOUND_LISTENER_H
#define SOUND_LISTENER_H

#include "GaiaComponent.h"
#include "SoundSystem.h"
#include "Vector3.h"

class SoundListener : public GaiaComponent
{
private:
	

public:
	SoundListener(GameObject* gameObject);
	~SoundListener();

	virtual void update(float deltaTime);
};

#endif