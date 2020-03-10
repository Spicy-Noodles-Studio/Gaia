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

#include <vector>

class SoundEmitter;
class SoundListener;

class SoundSystem : public Singleton<SoundSystem>
{
private:
	FMOD::System* system;
	FMOD_RESULT result;
	std::string file;
	std::map<std::string, FMOD::Sound*> sounds;

	FMOD::ChannelGroup* music;
	FMOD::ChannelGroup* soundEfects;

	std::vector<SoundEmitter*> *emitters;
	SoundListener *_listener =nullptr;
	

	void ERRCHECK(FMOD_RESULT result);
public:
	SoundSystem();
	~SoundSystem();

	void initSystem();
	bool createSounds(const std::string filename);
	
	FMOD::Channel* playSound(const std::string sound);
	FMOD::Channel* playMusic(const std::string sound);

	void setPauseAllSounds(bool pause);

	void setMusicVolume(float volume);
	void setSoundEffectsVolume(float volume);
	void setGeneralVolume(float volume);
	void setListenerAttributes(const Vector3& position, const Vector3& Forward, const Vector3& Up);

	void initListener(SoundListener* listener);
	void addEmitter(SoundEmitter* emitter);

	void update();
	// Utils
	FMOD_VECTOR vecToFMOD(const Vector3 &in);
	FMOD::Reverb3D* createReverb();
};

#endif