#include "ComponentData.h"
#include <stdio.h>

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

void ComponentData::addProperty(const std::string& propName, const std::string& propValue)
{
	if (properties.find(propName) != properties.end()) 
	{
		printf("ERROR: se ha intentado añadir al componente %s una propiedad %s que ya tiene definida.\n", name, propName);
		return;
	}
	properties[propName] = propValue;
}

void ComponentData::modifyProperty(const std::string& propName, const std::string& propValue)
{
	if (properties.find(propName) == properties.end())
	{
		printf("ERROR: se ha intentado modificar en el componente %s una propiedad %s que no tiene definida.\n", name, propName);
		return;
	}
	properties[propName] = propValue;
}

const std::string& ComponentData::getName() const
{
	return name;
}

const std::map<std::string, std::string>& ComponentData::getProperties() const
{
	return properties;
}
