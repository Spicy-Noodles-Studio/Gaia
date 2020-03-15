#pragma once
#ifndef SOUND_SYSTEM_H
#define SOUND_SYSTEM_H

#include "Vector3.h"


#include <fmod.h>
#include <fmod.hpp>
#include <map>
#include "Singleton.h"
#include <string.h>
#include <string>
#include "Quaternion.h"
#include <vector>



class GAIA_API SoundSystem : public Singleton<SoundSystem>
{
public:
	struct EmitterData
	{
		FMOD::Channel* channel;
		const Vector3* position;
		bool paused;
	};
	struct ListenerData
	{
		const Vector3* position;
		const Quaternion* quaternion;
	};
private:
	FMOD::System* system;
	FMOD_RESULT result;
	std::string file;
	std::map<std::string, FMOD::Sound*> sounds;

	FMOD::ChannelGroup* music;
	FMOD::ChannelGroup* soundEfects;

	std::vector<EmitterData*> emitters;
	ListenerData* listener = nullptr;

	void ERRCHECK(FMOD_RESULT result);
public:
	SoundSystem();
	~SoundSystem();

	void init();
	void close();

	bool createSounds(const std::string filename);

	FMOD::Channel* playSound(const std::string & sound);
	FMOD::Channel* playMusic(const std::string & sound);

	void setPauseAllSounds(bool pause);

	void setMusicVolume(float volume);
	void setSoundEffectsVolume(float volume);
	void setGeneralVolume(float volume);
	void setListenerAttributes(const Vector3& position, const Vector3& forward, const Vector3& up);

	void removeEmitter(EmitterData* emitter);
	void removeListener();

	void update(float deltaTime);

	EmitterData* createEmitter(const Vector3* pos);
	ListenerData* createListener(const Vector3* pos,const Quaternion* q);
	// Utils
	FMOD_VECTOR vecToFMOD(const Vector3& in);
	FMOD::Reverb3D* createReverb();
};

#endif