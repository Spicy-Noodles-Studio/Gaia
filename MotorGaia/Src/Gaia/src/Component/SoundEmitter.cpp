#include "SoundEmitter.h"

#include <sstream>
#include "GameObject.h"
#include "ComponentRegister.h"

REGISTER_FACTORY(SoundEmitter);

SoundEmitter::SoundEmitter(GameObject* gameObject) : GaiaComponent(gameObject), emitterData(nullptr), pitch(0.0f), volume(0.0f)
{
	SoundSystem* soundSystem = SoundSystem::GetInstance();
	checkNullAndBreak(soundSystem);

	checkNullAndBreak(gameObject);
	checkNullAndBreak(gameObject->transform);
	emitterData = soundSystem->createEmitter(&gameObject->transform->getPosition());
	pitch = 1.0f;
	volume = 1.0f;
}

SoundEmitter::~SoundEmitter()
{
	SoundSystem* soundSystem = SoundSystem::GetInstance();
	checkNullAndBreak(soundSystem);

	checkNullAndBreak(emitterData);
	soundSystem->removeEmitter(emitterData);
}

void SoundEmitter::playSound(const std::string& soundName, bool reverb)
{
	stop(soundName);
	checkNullAndBreak(emitterData);
	emitterData->channels[soundName] = new SoundChannel(SoundSystem::GetInstance()->playSound(soundName));
	setUpChannel(emitterData->channels[soundName], reverb);
}

void SoundEmitter::playMusic(const std::string& soundName, bool reverb)
{
	stop(soundName);
	checkNullAndBreak(emitterData);
	emitterData->channels[soundName] = new SoundChannel(SoundSystem::GetInstance()->playMusic(soundName));
	setUpChannel(emitterData->channels[soundName], reverb);
}

void SoundEmitter::stop(const std::string& sound)
{
	checkNullAndBreak(emitterData);
	auto it = emitterData->channels.find(sound);
	if (it != emitterData->channels.end()) {
		it->second->channel->stop();
		delete it->second;
		emitterData->channels.erase(it);
	}
}

void SoundEmitter::pause(const std::string& sound)
{
	checkNullAndBreak(emitterData);
	bool p;
	auto it = emitterData->channels.find(sound);
	if (it != emitterData->channels.end()) {
		SoundChannel* soundChannel = (*it).second;
		checkNullAndBreak(soundChannel);
		soundChannel->channel->getPaused(&p);
		if (!p) {
			soundChannel->channel->setPaused(true);
			soundChannel->paused = true;
		}
	}
}

void SoundEmitter::resume(const std::string& sound)
{
	checkNullAndBreak(emitterData);
	bool p;
	auto it = emitterData->channels.find(sound);
	if (it != emitterData->channels.end()) {
		SoundChannel* soundChannel = (*it).second;
		checkNullAndBreak(soundChannel);
		soundChannel->channel->getPaused(&p);
		if (p) {
			soundChannel->channel->setPaused(false);
			soundChannel->paused = false;
		}
	}
}

void SoundEmitter::stopAll()
{
	checkNullAndBreak(emitterData);
	for (auto it = emitterData->channels.begin(); it != emitterData->channels.end();)
		stop((*it++).first);
}

void SoundEmitter::pauseAll()
{
	checkNullAndBreak(emitterData);
	for (auto it = emitterData->channels.begin(); it != emitterData->channels.end(); it++)
		pause((*it).first);
}

void SoundEmitter::resumeAll()
{
	checkNullAndBreak(emitterData);
	for (auto it = emitterData->channels.begin(); it != emitterData->channels.end(); it++)
		resume((*it).first);
}

void SoundEmitter::setVolume(float volume)
{
	checkNullAndBreak(emitterData);
	this->volume = volume;
	for (auto it = emitterData->channels.begin(); it != emitterData->channels.end(); it++)
		setVolume(volume, (*it).first);
}

void SoundEmitter::setVolume(float volume, const std::string& sound)
{
	checkNullAndBreak(emitterData);
	auto it = emitterData->channels.find(sound);
	if (it != emitterData->channels.end()) 
		(*it).second->channel->setVolume(volume);
}

void SoundEmitter::setPitch(float pitch)
{
	checkNullAndBreak(emitterData);
	this->pitch = pitch;
	for (auto it = emitterData->channels.begin(); it != emitterData->channels.end(); it++)
		setPitch(pitch, (*it).first);
}

void SoundEmitter::setPitch(float pitch, const std::string& sound)
{
	checkNullAndBreak(emitterData);
	auto it = emitterData->channels.find(sound);
	if (it != emitterData->channels.end())
		(*it).second->channel->setPitch(pitch);
}

bool SoundEmitter::isPlaying(const std::string& soundName) const
{
	checkNullAndBreak(emitterData, false);
	bool playing = false;
	auto it = emitterData->channels.find(soundName);
	if (it != emitterData->channels.end())
		(*it).second->channel->isPlaying(&playing);
	return playing;
}

void SoundEmitter::setUpChannel(SoundChannel* soundChannel, bool reverb)
{
	if (soundChannel != nullptr && soundChannel->channel != nullptr) {
		soundChannel->channel->setPitch(pitch);
		soundChannel->channel->setVolume(volume);

		if (!reverb)
			soundChannel->channel->setReverbProperties(0, 0);

		soundChannel->paused = false;
	}
}

void SoundEmitter::handleData(ComponentData* data)
{
	checkNullAndBreak(data);
	for (auto prop : data->getProperties()) {
		std::stringstream ss(prop.second);

		if (prop.first == "volume") {
			float volume;
			if (ss >> volume)
				setVolume(volume);
			else
				LOG_ERROR("SOUND EMITTER", "Wrong value for property %s", prop.first.c_str());
		}
		else if (prop.first == "pitch") {
			float pitch;
			if (ss >> pitch)
				setPitch(pitch);
			else
				LOG_ERROR("SOUND EMITTER", "Wrong value for property %s", prop.first.c_str());
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
			LOG_ERROR("SOUND EMITTER", "Invalid property name \"%s\"", prop.first.c_str());
		}
	}

}
