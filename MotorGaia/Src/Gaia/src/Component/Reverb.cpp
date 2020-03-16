#include "Reverb.h"
#include "GameObject.h"
#include "SoundSystem.h"
#include <sstream>

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

	initPresets();

}

Reverb::~Reverb()
{
	reverb->release();
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
			
			if (presets.find(prop.second) != presets.end())
			{
				reverbProperties = presets[prop.second];
			}
			
			else LOG("REVERB: error loading preset %s\n", prop.second.c_str());
		}
		else if (prop.first == "maxDistance") {
			float maxDistance; ss >> maxDistance;
			setReverbMaxDistance(maxDistance);
		}
		else if (prop.first == "minDistance") {
			float minDistance; ss >> minDistance;
			setReverbMinDistance(minDistance);
		}
		else
		{
			LOG("REVERB: Invalid property name \"%s\"", prop.first.c_str());
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


void Reverb::initPresets()
{
	presets["off"] = FMOD_PRESET_OFF;
	presets["generic"] = FMOD_PRESET_GENERIC;
	presets["paddedcell"] = FMOD_PRESET_PADDEDCELL;
	presets["room"] = FMOD_PRESET_ROOM;
	presets["bathroom"] = FMOD_PRESET_BATHROOM;
	presets["livingroom"] = FMOD_PRESET_LIVINGROOM;
	presets["stoneroom"] = FMOD_PRESET_STONEROOM;
	presets["auditorium"] = FMOD_PRESET_AUDITORIUM;
	presets["concerthall"] = FMOD_PRESET_CONCERTHALL;
	presets["cave"] = FMOD_PRESET_CAVE;
	presets["arena"] = FMOD_PRESET_ARENA;
	presets["hangar"] = FMOD_PRESET_HANGAR;
	presets["carpettedhallway"] = FMOD_PRESET_CARPETTEDHALLWAY;
	presets["hallway"] = FMOD_PRESET_HALLWAY;
	presets["stonecorridor"] = FMOD_PRESET_STONECORRIDOR;
	presets["alley"] = FMOD_PRESET_ALLEY;
	presets["forest"] = FMOD_PRESET_FOREST;
	presets["city"] = FMOD_PRESET_CITY;
	presets["mountains"] = FMOD_PRESET_MOUNTAINS;
	presets["quarry"] = FMOD_PRESET_QUARRY;
	presets["plain"] = FMOD_PRESET_PLAIN;
	presets["parkinglot"] = FMOD_PRESET_PARKINGLOT;
	presets["sewerpipe"] = FMOD_PRESET_SEWERPIPE;
	presets["underwater"] = FMOD_PRESET_UNDERWATER;
}
