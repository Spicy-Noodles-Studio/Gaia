#include "GaiaData.h"

#include <fstream>
#include "DebugUtils.h"

GaiaData::GaiaData()
{

}

GaiaData::~GaiaData()
{

}

void GaiaData::create(nlohmann::json data)
{
	this->data = data;
}

void GaiaData::load(const std::string& filename)
{
	std::fstream fs;
	fs.open(filename);
	if (!fs.is_open()) {
		LOG_ERROR("GAIA DATA", "Filename \"%s\" not found", filename.c_str());
		return;
	}

	// Try if format file is valid
	try {
		fs >> data;
	}
	catch (std::exception message) {
		LOG_ERROR("GAIA DATA", "File \"%s\" invalid format. Should be .json formatting", filename.c_str());
		data.clear();
		fs.close();
		return;
	}
	fs.close();
}

GaiaData GaiaData::find(const std::string& key)
{
	GaiaData gaiaData = GaiaData();
	nlohmann::json::const_iterator it = data.find(key);
	if (it == data.end())
		LOG("\"%s\" keyword not found.", key.c_str());
	else
	{
		gaiaData.create(*it);
		LOG("\"%s\" keyword found. Loading...", key.c_str());
	}

	return gaiaData;
}

GaiaData GaiaData::operator[](int index) const
{
	GaiaData gaiaData = GaiaData();
	if (data.is_array() && index < data.size())
		gaiaData.create(data[index]);
	else
		LOG_ERROR("Index out of range.");
	return gaiaData;
}

int GaiaData::size()
{
	if (data.is_array())
		return data.size();
	else
		LOG_ERROR("GaiaData is not an array.");

	return 0;
}

GaiaData::iterator GaiaData::begin()
{
	return iterator(data.begin());
}

GaiaData::iterator GaiaData::end()
{
	return iterator(data.end());
}

std::string GaiaData::getValue()
{
	if (data.is_string())
		return data;
	return "";
}
