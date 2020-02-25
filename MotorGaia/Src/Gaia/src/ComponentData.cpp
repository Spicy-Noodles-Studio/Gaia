#include "ComponentData.h"7
#include <stdio.h>

ComponentData::ComponentData()
{
}

ComponentData::~ComponentData()
{
}

inline void ComponentData::setName(const std::string& componentName)
{
	name = componentName;
}

inline void ComponentData::setProperty(const std::string& propName, const std::string& propValue)
{
	if (properties.find(propName) != properties.end()) 
	{
		printf("ERROR: se ha intentado añadir al componente %s una propiedad %s que ya tiene definida\n", name, propName);
		return;
	}
	properties[propName] = propValue;
}

inline const std::string& ComponentData::getName() const
{
	return name;
}

inline const std::map<std::string, std::string>& ComponentData::getProperties() const
{
	return properties;
}
