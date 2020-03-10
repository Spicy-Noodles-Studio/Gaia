#include "SoundSystem.h"
#include <fmod_errors.h>
#include <string.h>
#include <string>

#include <iostream>
#include <fstream>
#include <sstream>
#include "SoundListener.h"
#include "SoundEmitter.h"

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

	std::cout << "\nSound system started" << "\n";

	createSounds(".//Assets//Sounds//sounds.asset");

	emitters = new std::vector<SoundEmitter*>();
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

		// Lectura de línea
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


FMOD::Channel* SoundSystem::playSound(const std::string sound)
{
	FMOD::Channel* channel;

	result = system->playSound(sounds[sound], soundEfects, false, &channel);
	ERRCHECK(result);
	channel->set3DMinMaxDistance(50, 10000);
	return channel;

}


FMOD::Channel* SoundSystem::playMusic(const std::string sound)
{
	FMOD::Channel* channel;

	result = system->playSound(sounds[sound], music, false, &channel);
	ERRCHECK(result);
	
	channel->set3DMinMaxDistance(50, 10000);
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

void SoundSystem::setListenerAttributes(const Vector3& position, const Vector3& Forward, const Vector3& Up)
{
	FMOD_VECTOR pos, vel, forward, up;
	pos = { float(position.x) ,float(position.y) ,float(position.z) };
	vel = { 0,0,0 };
	forward = { float(Forward.x) ,float(Forward.y) ,float(Forward.z) };
	up = { float(Up.x) ,float(Up.y) ,float(Up.z) };
	system->set3DListenerAttributes(0, &pos, &vel, &forward, &up);
	
}

void SoundSystem::initListener(SoundListener* listener)
{
	_listener = listener;
}

void SoundSystem::addEmitter(SoundEmitter* emitter)
{
	emitters->push_back(emitter);
}

void SoundSystem::update()
{
	_listener->update();
	for (int i = 0; i < emitters->size(); i++)
	{
		emitters->at(i)->update();
	}
}

FMOD_VECTOR SoundSystem::vecToFMOD(const Vector3& in)
{
	FMOD_VECTOR result;
	result.x = in.x;
	result.y = in.y;
	result.z = in.z;

	return result;
}

FMOD::Reverb3D*  SoundSystem::createReverb()
{
	FMOD::Reverb3D* reverb;
	result=system->createReverb3D(&reverb);
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
