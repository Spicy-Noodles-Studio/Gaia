#include "SoundSystem.h"
#include <fmod_errors.h>
#include <string.h>
#include <string>

#include <iostream>
#include <fstream>
#include <sstream>


SoundSystem::SoundSystem()
{
}

SoundSystem::~SoundSystem()
{
}

void SoundSystem::init()
{
	FMOD_RESULT result = FMOD::System_Create(&system);
	ERRCHECK(result);

	result = system->init(128, FMOD_INIT_NORMAL, 0);
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

	std::cout << "\nSound system started" << "\n";
}

void SoundSystem::close()
{
	for (auto sound : sounds) {
		sound.second->release();
		sound.second = nullptr;
	}
	sounds.clear();

	for (EmitterData* emi : emitters)
	{
		removeEmitter(emi);
	}
	removeListener();
	system->close();
	system->release();

	destroy();
}


bool SoundSystem::createSounds(const std::string filename)
{
	std::fstream stream;
	stream.open(filename);
	if (!stream.is_open()) {
		std::cout << "ERROR: sound.assets not found at route " << filename << "\n\n";
		return false;
	}

	std::string line;
	std::string path = ".//Assets//Sounds//";

	std::string file_path;
	std::string soundfile_name;
	std::string sound_name;
	std::string sound_mode;
	std::string loop;

	FMOD_MODE mode = FMOD_DEFAULT;
	FMOD::Sound* sonido;

	// Skip first two lines (Usage & example)
	std::getline(stream, line);
	std::getline(stream, line);

	int i = 0;
	while (std::getline(stream, line)) {

		// Lectura de l�nea
		std::istringstream iss(line);
		iss >> soundfile_name;
		iss >> sound_name;
		iss >> sound_mode;
		iss >> loop;
		iss.str(std::string());

		file_path = path + soundfile_name;

		mode = sound_mode == "3D" ? FMOD_3D : FMOD_2D;
		mode |= loop == "true" ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF;
		//mode |= FMOD_CREATESTREAM;
		FMOD_RESULT result;
		if ((result = system->createSound(file_path.c_str(), mode, 0, &sonido)) != FMOD_RESULT::FMOD_OK) {
			std::cout << "Sound loading failed at file: " << soundfile_name << "\n" <<
				"FMOD Error: " << FMOD_ErrorString(result) << "\n"
				<< "Sounds loaded before error: " << i << "\n\n";
			stream.close();
			return false;
		}
		sounds.emplace(sound_name, sonido);
		i++;
		std::cout << "Sound loaded: " << sound_name << "\n";

		//mode = FMOD_DEFAULT;
	}

	stream.close();

	std::cout << "All done" << "\n\n";

	return true;
}

Sound* SoundSystem::createSound(const std::string& soundName, const SoundMode& mode)
{
	Sound* sound;
	if (system->createSound(soundName.c_str(), mode, nullptr, &sound) == FMOD_RESULT::FMOD_OK)
		return sound;
	return nullptr;
}

FMOD::Channel* SoundSystem::playSound(const std::string & sound)
{
	Channel* channel;
	FMOD_RESULT result = system->playSound(sounds[sound], soundEfects, false, &channel);
	ERRCHECK(result);
	channel->set3DMinMaxDistance(50, 10000);
	return channel;
}

FMOD::Channel* SoundSystem::playMusic(const std::string & sound)
{
	Channel* channel;
	FMOD_RESULT result = system->playSound(sounds[sound], music, false, &channel);
	ERRCHECK(result);
	channel->set3DMinMaxDistance(50, 10000);
	return channel;
}

void SoundSystem::setPauseAllSounds(bool pause)
{
	ChannelGroup* master;
	FMOD_RESULT result = system->getMasterChannelGroup(&master);
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
	ChannelGroup* master;
	FMOD_RESULT result = system->getMasterChannelGroup(&master);
	ERRCHECK(result);
	master->setVolume(volume);
}

void SoundSystem::setListenerAttributes(const Vector3& position, const Vector3& forward, const Vector3& up)
{
	FMOD_VECTOR pos, vel, forwardTmp, upTmp;
	pos = { float(position.x) ,float(position.y) ,float(position.z) };
	vel = { 0,0,0 };
	forwardTmp = { float(forward.x) ,float(forward.y) ,float(forward.z) };
	upTmp = { float(up.x) ,float(up.y) ,float(up.z) };
	system->set3DListenerAttributes(0, &pos, &vel, &forwardTmp, &upTmp);
}


void SoundSystem::removeEmitter(EmitterData* emitter)
{
	auto it = std::find(emitters.begin(), emitters.end(), emitter);
	if (it != emitters.end())
	{
		delete* it;
		emitters.erase(it);
	}
}

void SoundSystem::removeListener()
{
	delete listener;
}

void SoundSystem::update(float deltaTime)
{
	Vector3 pos, forward, up;
	if (listener != nullptr)
	{
		pos = *listener->position;
		forward = GetForwardVector(*listener->quaternion);
		up = GetUpVector(*listener->quaternion);
		setListenerAttributes(pos, forward, up);
	}
	FMOD_VECTOR posEmi,zero;
	zero = { 0,0,0 };
	for (int i = 0; i < emitters.size(); i++)
	{
		if (emitters[i]->channel && !emitters[i]->paused) 
		{
			posEmi =vecToFMOD(*emitters[i]->position);
			emitters[i]->channel->set3DAttributes(&posEmi, &zero);
		}
	}

	FMOD_RESULT result = system->update();
	ERRCHECK(result);
}

SoundSystem::EmitterData* SoundSystem::createEmitter(const Vector3* pos)
{
	SoundSystem::EmitterData* data = new SoundSystem::EmitterData();
	data->position = pos;
	emitters.push_back(data);
	return data;
}

SoundSystem::ListenerData* SoundSystem::createListener(const Vector3* pos, const Quaternion* q)
{
	if (listener != nullptr)
		delete listener;
	ListenerData* data = new ListenerData();
	data->position = pos;
	data->quaternion = q;
	listener = data;
	return data;
}

FMOD_VECTOR SoundSystem::vecToFMOD(const Vector3& in)
{
	FMOD_VECTOR result;
	result.x = in.x;
	result.y = in.y;
	result.z = in.z;

	return result;
}

FMOD::Reverb3D* SoundSystem::createReverb()
{
	FMOD::Reverb3D* reverb;
	FMOD_RESULT result = system->createReverb3D(&reverb);
	ERRCHECK(result);
	return reverb;
}

void SoundSystem::ERRCHECK(FMOD_RESULT result)
{
	if (result != FMOD_RESULT::FMOD_OK)
	{
		std::cout << FMOD_ErrorString(result);
	}
}
