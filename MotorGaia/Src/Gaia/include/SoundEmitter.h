#pragma once
#ifndef SOUND_EMITTER_H
#define SOUND_EMITTER_H

#include "GaiaComponent.h"
#include "SoundSystem.h"
#include "Vector3.h"

class GAIA_API SoundEmitter : public GaiaComponent
{
private:
	SoundSystem::EmitterData * emitterData;

	float pitch;
	float volume;

public:
	SoundEmitter(GameObject* gameObject);
	~SoundEmitter();

	void playSound(std::string soundName, bool reverb = false);
	void playMusic(std::string soundName, bool reverb = false);
	void pause();
	void resume();

	void setVolume(float volume);
	void setPitch(float pitch);

	void handleData(ComponentData* data);
};

#endif

