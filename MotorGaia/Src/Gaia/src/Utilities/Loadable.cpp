#include "Loadable.h"
#include "DebugUtils.h"
#include <fstream>
#include <stdlib.h>

Loadable::Loadable(void* alloc) : state(LoadState::INVALID), data(alloc), id("")
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
		fs >> fileData;
		char id[120];
		_splitpath_s(filename.c_str(), nullptr, 0, nullptr, 0, id, 20, nullptr, 0);
		this->id = id;
	}
	catch (std::exception message) {
		LOG_ERROR("LOADABLE","File \"%s\" invalid format. Should be .json formatting", filename.c_str());
		fileData.clear();
		fs.close();
	}
	fs.close();

	this->filename = filename;
	state = LoadState::LOCATED;
	LOG("File \"%s\" located. Ready to load", filename.c_str());
}

void Loadable::load()
{
	if (state != LoadState::LOCATED) {
		LOG_ERROR("LOADABLE", "Trying to load a non located file \"%s\"", filename.c_str());
		return;
	}

	LOG("File \"%s\" loading...", filename.c_str());
	state = LoadState::LOADING;
	if (!load_internal()) {
		LOG_ERROR("LOADABLE", "Error while loading \"%s\"", filename.c_str());
		state = LoadState::INVALID;
		return;
	}
	state = LoadState::READY;
	LOG("File \"%s\" loaded", filename.c_str());
}

void Loadable::loadAsync()
{
	if (state != LoadState::LOCATED) {
		LOG_ERROR("LOADABLE", "Trying to load a non located file \"%s\"", filename.c_str());
		return;
	}
	LOG("File \"%s\" loading...", filename.c_str());
	state = LoadState::LOADING;
	loadThread = std::thread(&Loadable::load_aync_internal, std::ref(*this));
}

void Loadable::load_aync_internal()
{
	if (!load_internal()) {
		LOG_ERROR("LOADABLE", "Error while loading \"%s\"", filename.c_str());
		state = LoadState::INVALID;
		return;
	}
	state = LoadState::READY;
	LOG("File \"%s\" loaded", filename.c_str());
}
