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

	/* Set the component name */
	void setName(const std::string& componentName);

	/* Add property if it does not exist */
	bool addProperty(const std::string& propName, const std::string& propValue);

	/* Modify a property if it does exist, if it doesn't, it would be added */
	bool modifyProperty(const std::string& propName, const std::string& propValue);

	/* Get the component name */
	const std::string& getName() const;

	/* Get all registered properties */
	const std::vector<std::pair<std::string, std::string>>& getProperties() const;

	/* Dinamically allocate empty ComponentData, remember to call delete */
	static ComponentData* empty();

public:
	/* Component name */
	std::string name;

	/* To make search faster */
	std::unordered_map<std::string, int> nameID;

	/* Component properties (name, value) */
	std::vector<std::pair<std::string, std::string>> properties;
};

#endif