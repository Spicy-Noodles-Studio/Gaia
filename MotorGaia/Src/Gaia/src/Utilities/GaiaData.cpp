#include "GaiaData.h"

#include <fstream>
#include <iomanip>
#include "DebugUtils.h"

GaiaData::GaiaData() : data()
{

}

GaiaData::~GaiaData()
{

}

void GaiaData::create(nlohmann::json data)
{
	this->data = data;
}

bool GaiaData::load(const std::string& filename)
{
	std::fstream fs;
	fs.open(filename);
	if (!fs.is_open()) {
		LOG_ERROR("GAIA DATA", "Filename \"%s\" not found", filename.c_str());
		return false;
	}

	// Try if format file is valid
	try {
		fs >> data;
	}
	catch (std::exception message) {
		LOG_ERROR("GAIA DATA", "File \"%s\" invalid format. Should be .json formatting", filename.c_str());
		data.clear();
		fs.close();
		return false;
	}
	fs.close();
	return true;
}

GaiaData GaiaData::find(const std::string& key) const
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

int GaiaData::size() const
{
	if (data.is_array())
		return data.size();
	else
		LOG_ERROR("GaiaData is not an array.");

	return 0;
}

void GaiaData::save(const std::string& filename) const
{
	std::ofstream fs;
	fs.open(filename);

	// Try if format file is valid
	try {
		fs << std::setw(4) << data << std::endl;
	}
	catch (std::exception message) {
		LOG_ERROR("GAIA DATA", "File \"%s\" invalid format. Should be .json formatting", filename.c_str());
		fs.close();
		return;
	}
	fs.close();
}

void GaiaData::addElement(const std::string& element, const GaiaData& value)
{
	data[element] = value.data;
}

void GaiaData::addElement(const std::string& element, const std::vector<GaiaData>& value)
{
	std::vector<nlohmann::json> dataVector;
	for (GaiaData d : value)
		dataVector.push_back(d.data);
	data[element] = dataVector;
}

GaiaData::iterator GaiaData::begin() const
{
	return iterator(data.begin());
}

GaiaData::iterator GaiaData::end() const 
{
	return iterator(data.end());
}

std::string GaiaData::getValue() const
{
	if (data.is_string())
		return data;
	return "";
}
