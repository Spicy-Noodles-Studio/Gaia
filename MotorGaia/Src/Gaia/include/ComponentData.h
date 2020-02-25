#pragma once

#ifndef COMPONENT_DATA_H
#define COMPONENT_DATA_H

#include <string>
#include <map>

class ComponentData
{
public:
	ComponentData();
	~ComponentData();

	inline void setName(const std::string& componentName);
	inline void setProperty(const std::string& propName, const std::string& propValue);

	inline const std::string& getName() const;
	inline const std::map<std::string, std::string>& getProperties() const;

public:
	std::string name;
	std::map<std::string, std::string> properties;
};

#endif