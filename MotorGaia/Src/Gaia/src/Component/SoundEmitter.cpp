#include "SoundEmitter.h"

#include <sstream>

#include "GameObject.h"
#include "ComponentRegister.h"

REGISTER_FACTORY(SoundEmitter);

SoundEmitter::SoundEmitter(GameObject* gameObject) : GaiaComponent(gameObject)
{
	emitterData = SoundSystem::GetInstance()->createEmitter(&gameObject->transform->getPosition());
	emitterData->channel = nullptr;
	pitch = 1.0f;
	volume = 1.0f;
	emitterData->paused = true;
}

SoundEmitter::~SoundEmitter()
{
	SoundSystem::GetInstance()->removeEmitter(emitterData);
}

void SoundEmitter::playSound(std::string soundName, bool reverb)
{
	if (emitterData->channel)
		emitterData->channel->stop();

	emitterData->channel = SoundSystem::GetInstance()->playSound(soundName);
	if (emitterData->channel) {
		emitterData->channel->setPitch(pitch);
		emitterData->channel->setVolume(volume);

		if (!reverb)
			emitterData->channel->setReverbProperties(0, 0);

		emitterData->paused = false;
	}
}

void SoundEmitter::playMusic(std::string soundName, bool reverb)
{
	if (emitterData->channel)
		emitterData->channel->stop();

	emitterData->channel = SoundSystem::GetInstance()->playMusic(soundName);
	if (emitterData->channel != nullptr) {
		emitterData->channel->setPitch(pitch);
		emitterData->channel->setVolume(volume);
		if (!reverb) {
			emitterData->channel->setReverbProperties(0, 0);
		}

		emitterData->paused = false;
	}
}

void SoundEmitter::pause()
{
	bool p;
	if (emitterData->channel) {
		emitterData->channel->getPaused(&p);
		if (!p) {
			emitterData->channel->setPaused(true);
			emitterData->paused = true;
		}
	}
}

void SoundEmitter::resume()
{
	bool p;
	if (emitterData->channel) {
		emitterData->channel->getPaused(&p);
		if (p) {
			emitterData->channel->setPaused(false);
			emitterData->paused = false;
		}
	}
}

void SoundEmitter::setVolume(float volume)
{
	this->volume = volume;
	if (emitterData->channel) emitterData->channel->setVolume(volume);
}

void SoundEmitter::setPitch(float pitch)
{
	this->pitch = pitch;
	if (emitterData->channel) emitterData->channel->setPitch(pitch);
}

void SoundEmitter::handleData(ComponentData* data)
{
	for (auto prop : data->getProperties()) {
		std::stringstream ss(prop.second);

		if (prop.first == "volume") {
			float volume;
			if (ss >> volume)
				setVolume(volume);
			else
				LOG("SOUND EMITTER: wrong value for property %s.\n", prop.first.c_str());
		}
		else if (prop.first == "pitch") {
			float pitch;
			if (ss >> pitch)
				setPitch(pitch);
			else
				LOG("SOUND EMITTER: wrong value for property %s.\n", prop.first.c_str());
		}
		else if (prop.first == "playSound")
		{
			playSound(prop.second, true);
		}
		else if (prop.first == "playMusic")
		{
			playMusic(prop.second);
		}
		else
		{
			LOG("SOUND EMITTER: Invalid property name \"%s\"", prop.first.c_str());
		}
	}

}
