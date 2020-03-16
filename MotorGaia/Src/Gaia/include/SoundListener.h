#pragma once
#ifndef SOUND_LISTENER_H
#define SOUND_LISTENER_H

#include "GaiaComponent.h"
#include "Vector3.h"

class GAIA_API SoundListener : public GaiaComponent
{
public:
	SoundListener(GameObject* gameObject);
	~SoundListener();
};

#endif