#include "SoundEmitter.h"
#include "GameObject.h"


SoundEmitter::SoundEmitter(GameObject* gameObject) : GaiaComponent(gameObject)
{
	pos = SoundSystem::GetInstance()->vecToFMOD(gameObject->transform->getPosition());
	SoundSystem::GetInstance()->addEmitter(this);
	channel = nullptr;
	pitch = 1.0f;
	volume = 1.0f;
	paused = true;
}

SoundEmitter::~SoundEmitter()
{
	SoundSystem::GetInstance()->removeEmitter(this);
}

void SoundEmitter::update()
{
	if (channel && !paused) {
		pos = SoundSystem::GetInstance()->vecToFMOD(gameObject->transform->getPosition());
		channel->set3DAttributes(&pos, &zero);
	}
}

void SoundEmitter::playSound(std::string soundName, bool reverb)
{
	if (channel)
		channel->stop();

	channel = SoundSystem::GetInstance()->playSound(soundName);
	channel->set3DAttributes(&pos, &zero);
	channel->setPitch(pitch);
	channel->setVolume(volume);

	if (!reverb)
		channel->setReverbProperties(0, 0);

	paused = false;
}

void SoundEmitter::playMusic(std::string soundName, bool reverb)
{
	bool paused;
	if (channel) {
		channel->stop();
	}
	channel = SoundSystem::GetInstance()->playMusic(soundName);
	channel->set3DAttributes(&pos, &zero);
	channel->setPitch(pitch);
	channel->setVolume(volume);
	if (!reverb) {
		channel->setReverbProperties(0, 0);
	}

	paused = false;
}

void SoundEmitter::pause()
{
	if (channel) {
		channel->getPaused(&paused);
		if (!paused) {
			channel->setPaused(true);
			paused = true;
		}
	}
}

void SoundEmitter::resume()
{
	if (channel) {
		channel->getPaused(&paused);
		if (paused) {
			channel->setPaused(false);
			paused = false;
		}
	}
}

void SoundEmitter::setVolume(float volume)
{
	this->volume = volume;
	if (channel) channel->setVolume(volume);
}

void SoundEmitter::setPitch(float pitch)
{
	this->pitch = pitch;
	if (channel)channel->setPitch(pitch);
}

void SoundEmitter::handleData(ComponentData* data)
{
	for (auto prop : data->getProperties()) {
		std::stringstream ss(prop.second);

		if (prop.first == "volume") {
			float volume; ss >> volume;
			setVolume(volume);
		}
		else if (prop.first == "pitch") {
			float pitch; ss >> pitch;
			setPitch(pitch);
		}
	}

	// esto esta mas feo que hacer merge sin squash & merge!! ***
	playSound("prueba2", true);
}
