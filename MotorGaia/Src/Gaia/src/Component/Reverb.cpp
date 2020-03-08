#include "Reverb.h"
#include "GameObject.h"

Reverb::Reverb(GameObject* gameObject) : GaiaComponent(gameObject)
{
	active = true;
	minDistance = 0.0f;
	maxDistance = 20.0f;

	reverbProperties = FMOD_PRESET_OFF;

	SoundSystem::GetInstance()->createReverb(reverb);

    pos = SoundSystem::GetInstance()->vecToFMOD(gameObject->transform->getPosition());
	reverb->set3DAttributes(&pos, minDistance, maxDistance);
	reverb->setActive(active);
	reverb->setProperties(&reverbProperties);

	//SoundSystem::GetInstance()->createReverb(reverb);
}

Reverb::~Reverb()
{
	reverb->release();
}

void Reverb::update(float deltaTime)
{
}


void Reverb::setReverbMaxDistance(float distance)
{
	maxDistance = distance;
	reverb->set3DAttributes(&pos, minDistance, maxDistance);
}

void Reverb::setReverbMinDistance(float distance)
{
	minDistance = distance;
	reverb->set3DAttributes(&pos, minDistance, maxDistance);
}

void Reverb::setReverbActive(bool _active)
{
	active = _active;
	reverb->setActive(active);
}

void Reverb::setReverbPreset(PRESET type)
{
	switch (type) {
	case OFF:
		reverbProperties = FMOD_PRESET_OFF;
		break;
	case GENERIC:
		reverbProperties = FMOD_PRESET_GENERIC;
		break;
	case PADDEDCELL:
		reverbProperties = FMOD_PRESET_PADDEDCELL;
		break;
	case ROOM:
		reverbProperties = FMOD_PRESET_ROOM;
		break;
	case BATHROOM:
		reverbProperties = FMOD_PRESET_BATHROOM;
		break;
	case LIVINGROOM:
		reverbProperties = FMOD_PRESET_LIVINGROOM;
		break;
	case STONEROOM:
		reverbProperties = FMOD_PRESET_STONEROOM;
		break;
	case AUDITORIUM:
		reverbProperties = FMOD_PRESET_AUDITORIUM;
		break;
	case CONCERTHALL:
		reverbProperties = FMOD_PRESET_CONCERTHALL;
		break;
	case CAVE:
		reverbProperties = FMOD_PRESET_CAVE;
		break;
	case ARENA:
		reverbProperties = FMOD_PRESET_ARENA;
		break;
	case HANGAR:
		reverbProperties = FMOD_PRESET_HANGAR;
		break;
	case CARPETTEDHALLWAY:
		reverbProperties = FMOD_PRESET_CARPETTEDHALLWAY;
		break;
	case HALLWAY:
		reverbProperties = FMOD_PRESET_HALLWAY;
		break;
	case STONECORRIDOR:
		reverbProperties = FMOD_PRESET_STONECORRIDOR;
		break;
	case ALLEY:
		reverbProperties = FMOD_PRESET_ALLEY;
		break;
	case FOREST:
		reverbProperties = FMOD_PRESET_FOREST;
		break;
	case CITY:
		reverbProperties = FMOD_PRESET_CITY;
		break;
	case MOUNTAINS:
		reverbProperties = FMOD_PRESET_MOUNTAINS;
		break;
	case QUARRY:
		reverbProperties = FMOD_PRESET_QUARRY;
		break;
	case PLAIN:
		reverbProperties = FMOD_PRESET_PLAIN;
		break;
	case PARKINGLOT:
		reverbProperties = FMOD_PRESET_PARKINGLOT;
		break;
	case SEWERPIPE:
		reverbProperties = FMOD_PRESET_SEWERPIPE;
		break;
	case UNDERWATER:
		reverbProperties = FMOD_PRESET_UNDERWATER;
		break;
	default:
		reverbProperties = FMOD_PRESET_OFF;
		break;

		reverb->setProperties(&reverbProperties);
	}
}