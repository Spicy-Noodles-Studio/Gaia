#pragma once
#ifndef SOUND_SYSTEM_H
#define SOUND_SYSTEM_H

#include <fmod.h>
#include <fmod.hpp>
#include <map>
#include "Singleton.h"
#include<string.h>

class SoundSystem : public Singleton<SoundSystem>
{
private:
	FMOD::System* system;
	FMOD_RESULT result;
	std::string file;
	std::map<std::string, FMOD::Sound*> sounds;

	FMOD::ChannelGroup* music;
	FMOD::ChannelGroup* soundEfects;
	

	void ERRCHECK(FMOD_RESULT result);
public:
	SoundSystem();
	~SoundSystem();

	void initSystem();
	void createSounds(const std::string fileWithSoundsDirecctions);
	
	FMOD::Channel* playSound(const std::string sound);
	FMOD::Channel* playMusic(const std::string sound);
	
	void updateSounds();

	void setPauseAllSounds(bool pause);

	void setMusicVolume(float volume);
	void setSoundEffectsVolume(float volume);
	void setGeneralVolume(float volume);

};

#endif