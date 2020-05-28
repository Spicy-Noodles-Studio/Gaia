#include "Reverb.h"

#include <sstream>
#include "SoundSystem.h"
#include "GameObject.h"

#include "ComponentRegister.h"

REGISTER_FACTORY(Reverb);

Reverb::Reverb(GameObject* gameObject) : GaiaComponent(gameObject), reverbActive(true), minDistance(0.0f), maxDistance(0.0f), reverbProperties(FMOD_PRESET_OFF),
										 position(), reverb(nullptr), presets()
{
	reverbActive = true;
	minDistance = 0.0f;
	maxDistance = 10000.0f;

	reverbProperties = FMOD_PRESET_OFF;

	SoundSystem* soundSystem = SoundSystem::GetInstance();
	checkNullAndBreak(soundSystem);

	reverb = soundSystem->createReverb();
	checkNullAndBreak(gameObject);
	checkNullAndBreak(gameObject->transform);
	position = soundSystem->vecToFMOD(gameObject->transform->getPosition());

	checkNullAndBreak(reverb);
	reverb->set3DAttributes(&position, minDistance, maxDistance);
	reverb->setActive(reverbActive);
	reverb->setProperties(&reverbProperties);

	initPresets();
}

Reverb::~Reverb()
{
	checkNullAndBreak(reverb);
	reverb->release();
}

void Reverb::setReverbMaxDistance(float distance)
{
	maxDistance = distance;
	checkNullAndBreak(reverb);
	reverb->set3DAttributes(&position, minDistance, maxDistance);
}

void Reverb::setReverbMinDistance(float distance)
{
	minDistance = distance;
	checkNullAndBreak(reverb);
	reverb->set3DAttributes(&position, minDistance, maxDistance);
}

void Reverb::setReverbActive(bool active)
{
	reverbActive = active;
	checkNullAndBreak(reverb);
	reverb->setActive(reverbActive);
}

void Reverb::handleData(ComponentData* data)
{
	checkNullAndBreak(data);

	for (auto prop : data->getProperties()) {
		std::stringstream ss(prop.second);

		if (prop.first == "preset") {
			if (presets.find(prop.second) != presets.end())
				reverbProperties = presets[prop.second];
			else 
				LOG_ERROR("REVERB", "Error loading preset %s", prop.second.c_str());
		}
		else if (prop.first == "maxDistance") {
			float maxDistance;
			if (ss >> maxDistance)
				setReverbMaxDistance(maxDistance);
			else
				LOG_ERROR("REVERB", "Wrong value for property %s", prop.first.c_str());
		}
		else if (prop.first == "minDistance") {
			float minDistance;
			if (ss >> minDistance)
				setReverbMinDistance(minDistance);
			else
				LOG_ERROR("REVERB", "Wrong value for property %s", prop.first.c_str());
		}
		else
			LOG_ERROR("REVERB", "Invalid property name \"%s\"", prop.first.c_str());
	}
}

void Reverb::setReverbPreset(PRESET type)
{
	switch (type) {
	case PRESET::OFF:
		reverbProperties = FMOD_PRESET_OFF;
		break;
	case PRESET::GENERIC:
		reverbProperties = FMOD_PRESET_GENERIC;
		break;
	case PRESET::PADDEDCELL:
		reverbProperties = FMOD_PRESET_PADDEDCELL;
		break;
	case PRESET::ROOM:
		reverbProperties = FMOD_PRESET_ROOM;
		break;
	case PRESET::BATHROOM:
		reverbProperties = FMOD_PRESET_BATHROOM;
		break;
	case PRESET::LIVINGROOM:
		reverbProperties = FMOD_PRESET_LIVINGROOM;
		break;
	case PRESET::STONEROOM:
		reverbProperties = FMOD_PRESET_STONEROOM;
		break;
	case PRESET::AUDITORIUM:
		reverbProperties = FMOD_PRESET_AUDITORIUM;
		break;
	case PRESET::CONCERTHALL:
		reverbProperties = FMOD_PRESET_CONCERTHALL;
		break;
	case PRESET::CAVE:
		reverbProperties = FMOD_PRESET_CAVE;
		break;
	case PRESET::ARENA:
		reverbProperties = FMOD_PRESET_ARENA;
		break;
	case PRESET::HANGAR:
		reverbProperties = FMOD_PRESET_HANGAR;
		break;
	case PRESET::CARPETTEDHALLWAY:
		reverbProperties = FMOD_PRESET_CARPETTEDHALLWAY;
		break;
	case PRESET::HALLWAY:
		reverbProperties = FMOD_PRESET_HALLWAY;
		break;
	case PRESET::STONECORRIDOR:
		reverbProperties = FMOD_PRESET_STONECORRIDOR;
		break;
	case PRESET::ALLEY:
		reverbProperties = FMOD_PRESET_ALLEY;
		break;
	case PRESET::FOREST:
		reverbProperties = FMOD_PRESET_FOREST;
		break;
	case PRESET::CITY:
		reverbProperties = FMOD_PRESET_CITY;
		break;
	case PRESET::MOUNTAINS:
		reverbProperties = FMOD_PRESET_MOUNTAINS;
		break;
	case PRESET::QUARRY:
		reverbProperties = FMOD_PRESET_QUARRY;
		break;
	case PRESET::PLAIN:
		reverbProperties = FMOD_PRESET_PLAIN;
		break;
	case PRESET::PARKINGLOT:
		reverbProperties = FMOD_PRESET_PARKINGLOT;
		break;
	case PRESET::SEWERPIPE:
		reverbProperties = FMOD_PRESET_SEWERPIPE;
		break;
	case PRESET::UNDERWATER:
		reverbProperties = FMOD_PRESET_UNDERWATER;
		break;
	default:
		reverbProperties = FMOD_PRESET_OFF;
		break;
	}
	checkNullAndBreak(reverb);
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
