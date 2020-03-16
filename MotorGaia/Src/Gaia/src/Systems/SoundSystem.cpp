#include "SoundSystem.h"
#include <fmod_errors.h>
#include "ResourcesManager.h"
#include "DebugUtils.h"


SoundSystem::SoundSystem()
{
}

SoundSystem::~SoundSystem()
{
}

void SoundSystem::init()
{
	FMOD_RESULT result = FMOD::System_Create(&system);
	ERRCHECK(result);

	result = system->init(128, FMOD_INIT_NORMAL, 0);
	ERRCHECK(result);

	result = system->createChannelGroup("music", &music);
	ERRCHECK(result);
	FMOD::ChannelGroup* master;
	result = system->getMasterChannelGroup(&master);
	ERRCHECK(result);

	result = master->addGroup(music);
	ERRCHECK(result);

	result = system->createChannelGroup("soundEffect", &soundEfects);
	ERRCHECK(result);

	result = master->addGroup(soundEfects);
	ERRCHECK(result);

	LOG("\nSOUND SYSTEM: System started\n");
}

void SoundSystem::close()
{
	for (EmitterData* emi : emitters)
	{
		removeEmitter(emi);
	}

	removeListener();
	system->close();
	system->release();

	destroy();
}


Sound* SoundSystem::createSound(const std::string& name, const SoundMode& mode)
{
	Sound* sound;
	if (system->createSound(name.c_str(), mode, nullptr, &sound) == FMOD_RESULT::FMOD_OK)
		return sound;
	return nullptr;
}

FMOD::Channel* SoundSystem::playSound(const std::string& name)
{
	Channel* channel;
	Sound* sound = ResourcesManager::getSound(name);

	// Wait till loaded
	FMOD_OPENSTATE state;
	do {
		sound->getOpenState(&state, nullptr, nullptr, nullptr);
	} while (state != FMOD_OPENSTATE_READY);

	FMOD_RESULT result = system->playSound(sound, soundEfects, false, &channel);
	ERRCHECK(result);
	channel->set3DMinMaxDistance(50, 10000);
	return channel;
}

FMOD::Channel* SoundSystem::playMusic(const std::string& name)
{
	Channel* channel;
	Sound* sound = ResourcesManager::getSound(name);

	// Wait till loaded
	FMOD_OPENSTATE state;
	do {
		sound->getOpenState(&state, nullptr, nullptr, nullptr);
	} while (state != FMOD_OPENSTATE_READY);

	FMOD_RESULT result = system->playSound(sound, music, false, &channel);
	ERRCHECK(result);
	channel->set3DMinMaxDistance(50, 10000);
	return channel;
}

void SoundSystem::setPauseAllSounds(bool pause)
{
	ChannelGroup* master;
	FMOD_RESULT result = system->getMasterChannelGroup(&master);
	ERRCHECK(result);
	master->setPaused(pause);
}

/*
Parametros en decimal para no romper timpanos
*/
void SoundSystem::setMusicVolume(float volume)
{
	music->setVolume(volume);
}

/*
Parametros en decimal para no romper timpanos
*/
void SoundSystem::setSoundEffectsVolume(float volume)
{
	soundEfects->setVolume(volume);
}

/*
Parametros en decimal para no romper timpanos
*/
void SoundSystem::setGeneralVolume(float volume)
{
	ChannelGroup* master;
	FMOD_RESULT result = system->getMasterChannelGroup(&master);
	ERRCHECK(result);
	master->setVolume(volume);
}

void SoundSystem::setListenerAttributes(const Vector3& position, const Vector3& forward, const Vector3& up)
{
	FMOD_VECTOR pos, vel, forwardTmp, upTmp;
	pos = { float(position.x) ,float(position.y) ,float(position.z) };
	vel = { 0,0,0 };
	forwardTmp = { float(forward.x) ,float(forward.y) ,float(forward.z) };
	upTmp = { float(up.x) ,float(up.y) ,float(up.z) };
	system->set3DListenerAttributes(0, &pos, &vel, &forwardTmp, &upTmp);
}


void SoundSystem::removeEmitter(EmitterData* emitter)
{
	auto it = std::find(emitters.begin(), emitters.end(), emitter);
	if (it != emitters.end())
	{
		delete* it;
		emitters.erase(it);
	}
}

void SoundSystem::removeListener()
{
	delete listener;
}

void SoundSystem::update(float deltaTime)
{
	Vector3 pos, forward, up;
	if (listener != nullptr)
	{
		pos = *listener->position;
		forward = GetForwardVector(*listener->quaternion);
		up = GetUpVector(*listener->quaternion);
		setListenerAttributes(pos, forward, up);
	}
	FMOD_VECTOR posEmi,zero;
	zero = { 0,0,0 };
	for (int i = 0; i < emitters.size(); i++)
	{
		if (emitters[i]->channel && !emitters[i]->paused) 
		{
			posEmi =vecToFMOD(*emitters[i]->position);
			emitters[i]->channel->set3DAttributes(&posEmi, &zero);
		}
	}

	FMOD_RESULT result = system->update();
	ERRCHECK(result);
}

SoundSystem::EmitterData* SoundSystem::createEmitter(const Vector3* pos)
{
	SoundSystem::EmitterData* data = new SoundSystem::EmitterData();
	data->position = pos;
	emitters.push_back(data);
	return data;
}

SoundSystem::ListenerData* SoundSystem::createListener(const Vector3* pos, const Quaternion* q)
{
	if (listener != nullptr)
		delete listener;
	ListenerData* data = new ListenerData();
	data->position = pos;
	data->quaternion = q;
	listener = data;
	return data;
}

FMOD_VECTOR SoundSystem::vecToFMOD(const Vector3& in)
{
	FMOD_VECTOR result;
	result.x = in.x;
	result.y = in.y;
	result.z = in.z;

	return result;
}

FMOD::Reverb3D* SoundSystem::createReverb()
{
	FMOD::Reverb3D* reverb;
	FMOD_RESULT result = system->createReverb3D(&reverb);
	ERRCHECK(result);
	return reverb;
}

void SoundSystem::ERRCHECK(FMOD_RESULT result)
{
	if (result != FMOD_RESULT::FMOD_OK)
	{
		LOG("%s\n", FMOD_ErrorString(result));
	}
}
