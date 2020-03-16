#pragma once

#ifndef COMPONENT_DATA_H
#define COMPONENT_DATA_H

#include <string>
#include <unordered_map>
#include "GaiaExports.h"

class GAIA_API ComponentData
{
public:
	ComponentData();
	~ComponentData();

	void setName(const std::string& componentName);
	bool addProperty(const std::string& propName, const std::string& propValue);

	bool modifyProperty(const std::string& propName, const std::string& propValue);

	const std::string& getName() const;
	const std::unordered_map<std::string, std::string>& getProperties() const;

	static ComponentData* empty();

public:
	std::string name;
	std::unordered_map<std::string, std::string> properties;
};

#endif