#pragma once
#ifndef REVERB_H
#define REVERB_H

#include "GaiaComponent.h"
#include "SoundSystem.h"
#include "Vector3.h"

enum PRESET {
	OFF, GENERIC, PADDEDCELL, ROOM, BATHROOM, LIVINGROOM, STONEROOM, AUDITORIUM, CONCERTHALL,
	CAVE, ARENA, HANGAR, CARPETTEDHALLWAY, HALLWAY, STONECORRIDOR, ALLEY, FOREST, CITY, MOUNTAINS, QUARRY,
	PLAIN, PARKINGLOT, SEWERPIPE, UNDERWATER
};

class Reverb : public GaiaComponent
{
private:

	FMOD_VECTOR pos;

	float minDistance;
	float maxDistance;

	bool active;

	FMOD::Reverb3D* reverb;
	FMOD_REVERB_PROPERTIES reverbProperties;

public:
	Reverb(GameObject* gameObject);
	~Reverb();

	void setReverbPreset(PRESET type);
	void setReverbMaxDistance(float distance);
	void setReverbMinDistance(float distance);

	void setReverbActive(bool _active);

	void handleData(ComponentData* data);
};

#endif