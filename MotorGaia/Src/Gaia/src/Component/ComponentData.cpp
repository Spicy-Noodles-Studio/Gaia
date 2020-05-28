#include "ComponentData.h"
#include <stdio.h>
#include "DebugUtils.h"

ComponentData::ComponentData()
{
}

ComponentData::~ComponentData()
{
}

void ComponentData::setName(const std::string& componentName)
{
	name = componentName;
}

bool ComponentData::addProperty(const std::string& propName, const std::string& propValue)
{
	if (nameID.find(propName) != nameID.end())
	{
		LOG_ERROR("COMPONENT DATA", "Tried to add to %s a property %s already defined", name.c_str(), propName.c_str());
		return false;
	}
	properties.push_back({ propName, propValue });
	nameID[propName] = properties.size() - 1;
	return true;
}

bool ComponentData::modifyProperty(const std::string& propName, const std::string& propValue)
{
	if (nameID.find(propName) == nameID.end())
	{
		LOG_ERROR("COMPONENT DATA", "Tried to modify in %s component an undefined property %s. It would be overwitten", name.c_str(), propName.c_str());
		return addProperty(propName, propValue);
	}
	properties[nameID[propName]].second = propValue;
	return true;
}

const std::string& ComponentData::getName() const
{
	return name;
}

const std::vector<std::pair<std::string, std::string>>& ComponentData::getProperties() const
{
	return properties;
}

ComponentData* ComponentData::empty()
{
	ComponentData* c = new ComponentData();
	c->setName("null");
	return c;
}
