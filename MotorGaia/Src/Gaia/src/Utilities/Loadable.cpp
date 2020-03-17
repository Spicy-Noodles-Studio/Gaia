#include "Loadable.h"
#include "DebugUtils.h"
#include <fstream>

Loadable::Loadable() : state(LoadState::INVALID), data()
{
}
Loadable::~Loadable()
{
}

Loadable::LoadState Loadable::getLoadState()
{
	return state;
}

void* Loadable::getData()
{
	return data;
}

void Loadable::locate(std::string filename)
{
	std::fstream fs;
	fs.open(filename);
	if (!fs.is_open()) {
		LOG_ERROR("LOADABLE","Filename \"%s\" not found", filename.c_str());
	}

	// Try if format file is valid
	try {
		fs >> rawData;
	}
	catch (std::exception message) {
		LOG_ERROR("LOADABLE","File \"%s\" invalid format. Should be .json formatting", filename.c_str());
		rawData.clear();
		fs.close();
	}
	fs.close();

	this->filename = filename;
	state = LoadState::LOCATED;
	LOG("File \"%s\" located. Ready to load", filename.c_str());
}

void Loadable::load()
{
	state = LoadState::LOADING;
	if (!load_internal()) {
		LOG_ERROR("LOADABLE", "Error while loading \"%s\"", filename.c_str());
		state = LoadState::INVALID;
		return;
	}
	state = LoadState::READY;
}

void Loadable::loadAsync()
{
	state = LoadState::LOADING;
	loadThread = std::thread(&load_aync_internal);
}

void Loadable::load_aync_internal()
{
	if (!load_internal()) {
		LOG_ERROR("LOADABLE", "Error while loading \"%s\"", filename.c_str());
		state = LoadState::INVALID;
		return;
	}
	state = LoadState::READY;
}
