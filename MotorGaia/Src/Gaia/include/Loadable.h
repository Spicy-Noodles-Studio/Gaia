#pragma once
#ifndef LOADABLE_H
#define LOADABLE_H

#include <string>
#include <nlohmann\json.hpp>
#include <thread>

typedef nlohmann::json RawData;

class Loadable
{
public:
	enum class LoadState
	{
		INVALID = -1,
		LOCATED,
		LOADING,
		READY
	};

protected:
	LoadState state;
	RawData fileData;

private:
	std::string filename;
	void* data;

	std::thread loadThread;

public:
	Loadable(void* alloc);
	virtual ~Loadable();

	/*	Return the state of the loadable 
	 *	returns INVALID does not exist or failed 
	 *	returns LOCATED when loadable was located
	 *	returns LOADING when loadable is loading
	 *	returns READY when loadable is loaded and ready to use
	 */
	LoadState getLoadState();
	void* getData();

protected:
	/* Has to be defined, use rawData(json) */
	virtual bool load_internal() = 0;

	// TODO: cambiar a private
public:
	void locate(std::string filename);
	void load();
	void loadAsync();

	void load_aync_internal();
};


#endif