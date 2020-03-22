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
	if (properties.find(propName) != properties.end()) 
	{
		LOG("COMPONENT DATA: tried to add to %s a property %s already defined\n", name.c_str(), propName.c_str());
		return false;
	}
	properties[propName] = propValue;
	return true;
}

bool ComponentData::modifyProperty(const std::string& propName, const std::string& propValue)
{
	if (properties.find(propName) == properties.end())
	{
		LOG_ERROR("COMPONENT DATA", "Tried to modify in %s component an undefined property %s. It would be overwitten", name.c_str(), propName.c_str());
		//return false;
	}
	properties[propName] = propValue;
	return true;
}

const std::string& ComponentData::getName() const
{
	return name;
}

const std::unordered_map<std::string, std::string>& ComponentData::getProperties() const
{
	return properties;
}

ComponentData* ComponentData::empty()
{
	ComponentData* c = new ComponentData();
	c->setName("null");
	return c;
}
