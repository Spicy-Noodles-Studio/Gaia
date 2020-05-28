#pragma once
#ifndef SOUND_LISTENER_H
#define SOUND_LISTENER_H

#include "GaiaComponent.h"

class GAIA_API SoundListener : public GaiaComponent
{
public:
	SoundListener(GameObject* gameObject);
	virtual ~SoundListener();

};

#endif