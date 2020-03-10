#include "Reverb.h"
#include "GameObject.h"

Reverb::Reverb(GameObject* gameObject) : GaiaComponent(gameObject)
{
	active = true;
	minDistance = 0.0f;
	maxDistance = 10000.0f;

	reverbProperties = FMOD_PRESET_OFF;

	reverb = SoundSystem::GetInstance()->createReverb();

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

void Reverb::handleData(ComponentData* data)
{
	for (auto prop : data->getProperties()) {
		std::stringstream ss(prop.second);

		if (prop.first == "preset") {
			if (prop.second == "off") reverbProperties = FMOD_PRESET_OFF;
			else if (prop.second == "generic") reverbProperties = FMOD_PRESET_GENERIC;
			else if (prop.second == "paddedcell") reverbProperties = FMOD_PRESET_PADDEDCELL;
			else if (prop.second == "room") reverbProperties = FMOD_PRESET_ROOM;
			else if (prop.second == "bathroom") reverbProperties = FMOD_PRESET_BATHROOM;
			else if (prop.second == "livingroom") reverbProperties = FMOD_PRESET_LIVINGROOM;
			else if (prop.second == "stoneroom") reverbProperties = FMOD_PRESET_STONEROOM;
			else if (prop.second == "auditorium") reverbProperties = FMOD_PRESET_AUDITORIUM;
			else if (prop.second == "concerthall") reverbProperties = FMOD_PRESET_CONCERTHALL;
			else if (prop.second == "cave") reverbProperties = FMOD_PRESET_CAVE;
			else if (prop.second == "arena") reverbProperties = FMOD_PRESET_ARENA;
			else if (prop.second == "hangar") reverbProperties = FMOD_PRESET_HANGAR;
			else if (prop.second == "carpettedhallway") reverbProperties = FMOD_PRESET_CARPETTEDHALLWAY;
			else if (prop.second == "hallway") reverbProperties = FMOD_PRESET_HALLWAY;
			else if (prop.second == "stonecorridor") reverbProperties = FMOD_PRESET_STONECORRIDOR;
			else if (prop.second == "alley") reverbProperties = FMOD_PRESET_ALLEY;
			else if (prop.second == "forest") reverbProperties = FMOD_PRESET_FOREST;
			else if (prop.second == "city") reverbProperties = FMOD_PRESET_CITY;
			else if (prop.second == "mountains") reverbProperties = FMOD_PRESET_MOUNTAINS;
			else if (prop.second == "quarry") reverbProperties = FMOD_PRESET_QUARRY;
			else if (prop.second == "plain") reverbProperties = FMOD_PRESET_PLAIN;
			else if (prop.second == "parkinglot") reverbProperties = FMOD_PRESET_PARKINGLOT;
			else if (prop.second == "sewerpipe") reverbProperties = FMOD_PRESET_SEWERPIPE;
			else if (prop.second == "underwater") reverbProperties = FMOD_PRESET_UNDERWATER;
			else printf("REVERB: error loading preset %s\n", prop.second);
		}
		else if (prop.first == "maxDistance") {
			float maxDistance; ss >> maxDistance;
			setReverbMaxDistance(maxDistance);
		}
		else if (prop.first == "minDistance") {
			float minDistance; ss >> minDistance;
			setReverbMinDistance(minDistance);
		}
	}
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
	}
	reverb->setProperties(&reverbProperties);
}