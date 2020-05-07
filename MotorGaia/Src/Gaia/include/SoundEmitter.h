#pragma once
#ifndef SOUND_EMITTER_H
#define SOUND_EMITTER_H

#include "GaiaComponent.h"
#include "SoundSystem.h"
#include "Vector3.h"

typedef SoundSystem::SoundChannel SoundChannel;

class GAIA_API SoundEmitter : public GaiaComponent
{
private:
	SoundSystem::EmitterData * emitterData;

	float pitch;
	float volume;

	void stop(const std::string& sound);
	void setUpChannel(SoundChannel* channel, bool reverb);

public:
	SoundEmitter(GameObject* gameObject);
	~SoundEmitter();

	void playSound(const std::string& soundName, bool reverb = false);
	void playMusic(const std::string& soundName, bool reverb = false);
	void pause(const std::string& sound);
	void resume(const std::string& sound);
	void pauseAll();
	void resumeAll();

	void setVolume(float volume);
	void setVolume(float volume, const std::string& sound);
	void setPitch(float pitch);
	void setPitch(float pitch, const std::string& sound);

	bool isPlaying(const std::string& soundName);

	void handleData(ComponentData* data);
};

#endif

