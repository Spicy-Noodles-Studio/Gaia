#pragma once
#ifndef SOUND_SYSTEM_H
#define SOUND_SYSTEM_H

#include "Singleton.h"

#include <fmod.hpp>
#include <map>
#include <vector>
#include <string>
#include "Quaternion.h"
#include <vector>
#include "Vector3.h"

typedef FMOD_MODE SoundMode;
typedef FMOD::Sound Sound;
typedef FMOD::System System;
typedef FMOD::Channel Channel;
typedef FMOD::ChannelGroup ChannelGroup;

class GAIA_API SoundSystem : public Singleton<SoundSystem>
{
public:	
	struct SoundChannel {
		Channel* channel;
		bool paused;
		
		~SoundChannel();
		SoundChannel();
		SoundChannel(Channel* channel);
	};
	struct EmitterData
	{
		std::map<std::string, SoundChannel*> channels;
		const Vector3* position;	

		~EmitterData();
		EmitterData(const Vector3* position);
		bool isPaused();
	};
	struct ListenerData
	{
		const Vector3* position;
		const Quaternion* quaternion;
	};
private:
	System* system;

	ChannelGroup* music;
	ChannelGroup* soundEfects;

	float generalVolume;
	float soundVolume;
	float musicVolume;
	std::vector<EmitterData*> emitters;
	ListenerData* listener = nullptr;

	void ERRCHECK(FMOD_RESULT result);

	Sound* getSound(const std::string& name);
public:
	SoundSystem();
	~SoundSystem();

	void init();
	void close();

	Sound* createSound(const std::string& name, const SoundMode& mode);

	Channel* playSound(const std::string& name);
	Channel* playMusic(const std::string& name);

	void setPauseAllSounds(bool pause);

	void setMusicVolume(float volume);
	void setSoundEffectsVolume(float volume);
	void setGeneralVolume(float volume);
	void setListenerAttributes(const Vector3& position, const Vector3& forward, const Vector3& up);

	float getGeneralVolume();
	float getMusicVolume();
	float getSoundVolume();

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