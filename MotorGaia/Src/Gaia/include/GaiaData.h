#pragma once
#ifndef GAIA_DATA_H
#define GAIA_DATA_H

#include "GaiaExports.h"
#include <nlohmann\json.hpp>

class GAIA_API GaiaData
{
private:
	nlohmann::json data;

	void create(nlohmann::json data);

public:
	struct iterator
	{
	private:
		nlohmann::json::const_iterator it;
	public:
		iterator(nlohmann::json::const_iterator it) : it(it) {}
		bool operator!=(iterator other)
		{
			return it != other.it;
		}
		GaiaData operator*()
		{
			GaiaData gaiaData = GaiaData();
			gaiaData.create(*it);
			return gaiaData;
		}
		iterator& operator++()
		{
			++it;
			return *this;
		}
		iterator operator++(int index)
		{
			iterator temp = *this;
			operator++();
			return temp;
		}
	};

	GaiaData();
	~GaiaData();

	std::string getValue() const;

	bool load(const std::string& filename);
	GaiaData find(const std::string& key) const;
	GaiaData operator[](int index) const;
	int size() const;

	void save(const std::string& filename) const;

	void addElement(const std::string& element, const GaiaData& value);
	void addElement(const std::string& element, const std::vector<GaiaData>& value);
	
	template<typename T>
	void addElement(const std::string& element, const T& value);

	iterator begin() const;
	iterator end() const;

};
#endif

template<typename T>
inline void GaiaData::addElement(const std::string& element, const T& value)
{
	data[element] = value;
}
