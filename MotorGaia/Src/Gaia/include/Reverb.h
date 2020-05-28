#pragma once
#ifndef REVERB_H
#define REVERB_H
#include "GaiaComponent.h"
#include "fmod_common.h"
#include <map>
#include "Vector3.h"

namespace FMOD {
	class Reverb3D;
}

class GAIA_API Reverb : public GaiaComponent
{
public:
	enum class PRESET {
		OFF, GENERIC, PADDEDCELL, ROOM, BATHROOM, LIVINGROOM, STONEROOM, AUDITORIUM, CONCERTHALL,
		CAVE, ARENA, HANGAR, CARPETTEDHALLWAY, HALLWAY, STONECORRIDOR, ALLEY, FOREST, CITY, MOUNTAINS, QUARRY,
		PLAIN, PARKINGLOT, SEWERPIPE, UNDERWATER
	};

private:
	std::map<std::string, FMOD_REVERB_PROPERTIES> presets;
	FMOD_VECTOR position;

	float minDistance;
	float maxDistance;

	bool reverbActive;

	FMOD::Reverb3D* reverb;
	FMOD_REVERB_PROPERTIES reverbProperties;

public:
	Reverb(GameObject* gameObject);
	virtual ~Reverb();
	
	void setReverbPreset(PRESET type);
	void setReverbMaxDistance(float distance);
	void setReverbMinDistance(float distance);

	void setReverbActive(bool active);

protected:
	void handleData(ComponentData* data);

private:
	void initPresets();
};

#endif