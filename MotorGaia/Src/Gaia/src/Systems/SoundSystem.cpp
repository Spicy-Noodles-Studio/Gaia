#include "SoundSystem.h"
#include <iostream>
#include <fmod_errors.h>
#include<string.h>

SoundSystem::SoundSystem()
{
}

SoundSystem::~SoundSystem()
{
}

void SoundSystem::initSystem()
{
	result = FMOD::System_Create(&system);
	ERRCHECK(result);

	result = system->init(128, FMOD_INIT_NORMAL,0);
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

}

FMOD::Channel* SoundSystem::playSound(const std::string sound)
{
	FMOD::Channel* channel;

	result = system->playSound(sounds[sound], soundEfects, false, &channel);
	ERRCHECK(result);
	return channel;

}


FMOD::Channel* SoundSystem::playMusic(const std::string sound)
{
	FMOD::Channel* channel;

	result = system->playSound(sounds[sound], music, false, &channel);
	ERRCHECK(result);
	return channel;

}

void SoundSystem::updateSounds()
{
	result = system->update();
}

void SoundSystem::setPauseAllSounds(bool pause)
{
	FMOD::ChannelGroup* master;
	result=system->getMasterChannelGroup(&master);
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
	FMOD::ChannelGroup* master;
	result = system->getMasterChannelGroup(&master);
	ERRCHECK(result);
	master->setVolume(volume);
}

void SoundSystem::ERRCHECK(FMOD_RESULT result)
{
	if (result != FMOD_RESULT::FMOD_OK)
	{
		std::cout << FMOD_ErrorString(result);

	}
}
