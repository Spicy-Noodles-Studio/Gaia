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

	void setName(const std::string& componentName);
	void addProperty(const std::string& propName, const std::string& propValue);

	void modifyProperty(const std::string& propName, const std::string& propValue);

	const std::string& getName() const;
	const std::map<std::string, std::string>& getProperties() const;

public:
	std::string name;
	std::map<std::string, std::string> properties;
};

#endif