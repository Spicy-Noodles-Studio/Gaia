#pragma once
#ifndef SOUND_EMITTER_H
#define SOUND_EMITTER_H

#include "GaiaComponent.h"
#include "SoundSystem.h"
#include "Vector3.h"

class SoundEmitter : public GaiaComponent
{
private:

	FMOD_VECTOR pos;
	FMOD_VECTOR zero = { 0,0,0 };

	FMOD::Channel* channel;
	float pitch;
	float volume;

	bool paused;

public:
	SoundEmitter(GameObject* gameObject);
	~SoundEmitter();

	virtual void update();

	void playSound(std::string soundName, bool reverb = false);
	void playMusic(std::string soundName, bool reverb = false);
	void pause();
	void resume();

	void setVolume(float volume);
	void setPitch(float pitch);

	void handleData(ComponentData* data);
};

#endif

