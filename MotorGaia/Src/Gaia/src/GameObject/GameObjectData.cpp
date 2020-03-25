#include "GameObjectData.h"
#include "DebugUtils.h"
#include "ResourcesManager.h"
#include <stdio.h>


GameObjectData::GameObjectData() : blueprintRef(nullptr)
{
}

GameObjectData::GameObjectData(const GameObjectData& other)
{
	name = other.name;
	tag = other.tag;
	
	if (other.blueprintRef != nullptr) {
		while (other.blueprintRef->getLoadState() != Loadable::LoadState::READY); //Wait till loaded
		GameObjectData bp(*other.blueprintRef);

		for (auto c : bp.components)
			components[c.first] = new ComponentData(*c.second);

		for (auto c : bp.children)
			children[c.first] = new GameObjectData(*c.second);

		for (auto c : other.componentModifications)
			applyComponentModification(c.first, c.second);

		for (auto c : other.childrenModifications)
			applyChildModification(c.first, c.second);
	}

	for (auto c : other.components)
		components[c.first] = new ComponentData(*c.second);

	for (auto c : other.children)
		children[c.first] = new GameObjectData(*c.second);

}

GameObjectData::~GameObjectData()
{
	for (auto c : components) {
		delete c.second;
		c.second = nullptr;
	}

	for (auto c : children) {
		delete c.second;
		c.second = nullptr;
	}

	for (auto c : componentModifications) {
		delete c.second;
		c.second = nullptr;
	}

	for (auto c : childrenModifications) {
		delete c.second;
		c.second = nullptr;
	}

	components.clear();
	children.clear();
	componentModifications.clear();
	childrenModifications.clear();
}

bool GameObjectData::loadData(const RawData& data)
{
	nlohmann::json::const_iterator objectName = data.find("ObjectName");
	nlohmann::json::const_iterator objectTag = data.find("Tag");
	if (objectName == data.end() || objectTag == data.end()) {
		LOG_ERROR("GAMEOBJECT DATA", "Object name nor tag found");
		return false;
	}

	setName(*objectName);
	setTag(*objectTag);

	nlohmann::json::const_iterator objectType = data.find("ObjectType");
	if (objectType == data.end()) {
		LOG_ERROR("GAMEOBJECT DATA", "Object type not found %s", name.c_str());
		return false;
	}
	std::string type = *objectType;
	std::transform(type.begin(), type.end(), type.begin(), std::tolower);

	// Si es blueprint
	if (type == "blueprint") {
		// Buscar referencia
		nlohmann::json::const_iterator bpPath = data.find("BlueprintPath");
		if (bpPath == data.end()) {
			LOG_ERROR("BLUEPRINT DATA", "BlueprintPath keyword not found in \"%s\" blueprint", name.c_str());
			return false;
		}
		std::string bpName = *bpPath;
		// Coge la referencia del blueprint
		const BlueprintData* bpData = ResourcesManager::getBlueprint(bpName);
		if (bpData->getLoadState() == Loadable::LoadState::INVALID || bpData == nullptr)
			return false;

		setBlueprint(bpData);
		// Buscar modificicadores

		//Se modifican los componentes si es necesario
		nlohmann::json::const_iterator compoMod = data.find("ComponentModifications");
		if (compoMod != data.end()) {
			if (!addComponentModifications(*compoMod)) {
				LOG_ERROR("GAMEOBJECT DATA", "Blueprint \"%s\" ComponentModifications not loaded", name.c_str());
			}
		}

		//Se modifican los children si es necesario
		nlohmann::json::const_iterator childMod = data.find("ChildrenModifications");
		if (childMod != data.end()) {
			if (!addChildrenModifications(*childMod)) {
				LOG_ERROR("GAMEOBJECT DATA", "Blueprint \"%s\" ChildrenModifications not loaded", name.c_str());
			}
		}
	}
	// Tanto para gameobjects como blueprints, componentes e hijos

	// Se cargan los nuevos componentes
	nlohmann::json::const_iterator components = data.find("Components");
	if (components != data.end()) {
		LOG("\"Components\" keyword found. Loading...");
		if (!addComponents(components.value())) {
			LOG_ERROR("BLUEPRINT DATA", "Error loading \"Components\" in \"%s\"", name.c_str());
			return false;
		}
		LOG("\"Components\" loaded successfully");
	}

	// Se cargan los hijos
	nlohmann::json::const_iterator children = data.find("Children");
	if (children != data.end()) {
		LOG("\"Children\" keyword found. Loading...");
		if (!addChildren(children.value())) {
			LOG_ERROR("BLUEPRINT DATA", "Error loading \"Children\" in \"%s\"", name.c_str());
			return false;
		}
		LOG("\"Children\" loaded successfully");
	}

	return true;
}

bool GameObjectData::addComponentModifications(const RawData& data)
{
	// Para cada modificacion
	for (auto& mod : data) {
		nlohmann::json::const_iterator name = mod.find("ComponentName");
		if (name == mod.end()) {
			LOG_ERROR("GAMEOBJECT DATA", "ComponentName not found for modification");
			continue; // Si no se ha podido modificar el componente pasamos al siguiente
		}
		nlohmann::json::const_iterator properties = mod.find("ComponentProperties");
		if (properties == mod.end()) {
			LOG_ERROR("GAMEOBJECT DATA", "ComponentProperties not found for modification");
			continue;
		}
		if (componentModifications.find(*name) != componentModifications.end()) {
			LOG_ERROR("GAMEOBJECT DATA", "Trying to load a component modification that already was registered");
			continue;
		}

		// Todo bien, a�adimos
		ComponentData* cData = new ComponentData();
		componentModifications[*name] = cData;

		cData->setName(*name);
		// Por cada propiedad
		for (auto& prop : properties.value().items()) {
			std::string cName = prop.key();
			std::string cValue = prop.value();
			if (!cData->addProperty(prop.key(), prop.value())) {
				LOG_ERROR("GAMEOBJECT DATA", "Error trying to add a ComponentProperty: \"%s\" : \"%s\"", cName.c_str(), cValue.c_str());
				return false;
			}
		}
	}
	return true;
}

bool GameObjectData::addChildrenModifications(const RawData& data)
{
	// Para cada modificacion
	for (auto& mod : data.items()) {

		if (childrenModifications.find(mod.key()) != childrenModifications.end()) {
			LOG_ERROR("GAMEOBJECT DATA", "Trying to load a children modification \"%s\" that already was registered", mod.key().c_str());
			continue;
		}

		GameObjectData* childData = new GameObjectData();
		childrenModifications[mod.key()] = childData;

		// Modificaciones de componentes existentes
		nlohmann::json::const_iterator childCompMod = mod.value().find("ComponentModifications");
		if (childCompMod != mod.value().end())
			if(!childData->addComponentModifications(*childCompMod))
				LOG_ERROR("GAMEOBJECT DATA", "Adding ComponentModifications \"%s\" failed", childCompMod.key().c_str());

		// Modificaciones de hijos que existen
		nlohmann::json::const_iterator childModMod = mod.value().find("ChildrenModifications");
		if (childCompMod != mod.value().end())
			if (!childData->addChildrenModifications(*childModMod))
				LOG_ERROR("GAMEOBJECT DATA", "Adding ChildrenModifications \"%s\" failed", childCompMod.key().c_str());

		// A�adir componentes a los ya existentes
		nlohmann::json::const_iterator childComp = mod.value().find("Components");
		if (childComp != mod.value().end()) {
			LOG("\"Components\" keyword found. Loading...");
			if (!childData->addComponents(childComp.value())) {
				LOG_ERROR("GAMEOBJECT DATA", "Error loading \"Components\" in \"%s\"", name.c_str());
				return false;
			}
			LOG("\"Components\" loaded successfully");
		}

		// A�adir hijos a los ya existentes
		nlohmann::json::const_iterator childChildren = mod.value().find("Children");
		if (childChildren != mod.value().end()) {
			LOG("\"Children\" keyword found. Loading...");
			if (!childData->addChildren(childChildren.value())) {
				LOG_ERROR("GAMEOBJECT DATA", "Error loading \"Children\" in \"%s\"", name.c_str());
				return false;
			}
			LOG("\"Children\" loaded successfully");
		}
	}
	return true;
}

bool GameObjectData::addComponents(const RawData& data)
{
	for (auto& component : data) {
		nlohmann::json::const_iterator compName = component.find("ComponentName");
		if (compName == component.end()) {
			LOG_ERROR("GAMEOBJECT DATA", "ComponentName keyword not found");
			return false;
		}
		addComponent(component);
	}
	return true;
}

bool GameObjectData::addComponent(const RawData& data)
{
	nlohmann::json::const_iterator name = data.find("ComponentName");
	nlohmann::json::const_iterator properties = data.find("ComponentProperties");

	if (components.find(*name) != components.end()) {
		std::string nameAux = *name;
		LOG_ERROR("GAMEOBJECT DATA", "Trying to add an already exiting component \"%s\"", nameAux.c_str());
		return false;
	}

	ComponentData* cData = new ComponentData();
	components[*name] = cData;

	cData->setName(*name);
	if (properties != data.end()) {
		LOG("Loading %s properties", cData->getName().c_str());
		for (auto& property : (*properties).items()) {
			if (!cData->addProperty(property.key(), property.value())) {
				return false;
			}
		}
	}
	return true;
}

bool GameObjectData::addChildren(const RawData& data)
{
	for (auto& child : data) {
		nlohmann::json::const_iterator objectName = child.find("ObjectName");
		nlohmann::json::const_iterator objectType = child.find("ObjectType");
		if (objectName == child.end()) {
			LOG_ERROR("GAMEOBJECT DATA", "ObjectName keyword not found");
			return false;
		}
		if (objectType == child.end()) {
			LOG_ERROR("GAMEOBJECT DATA", "ObjectType keyword not found");
			return false;
		}
		addChild(child);
	}
	return true;
}

bool GameObjectData::addChild(const RawData& data)
{
	nlohmann::json::const_iterator objectName = data.find("ObjectName");

	if (children.find(*objectName) != children.end()) {
		LOG_ERROR("GAMEOBJECT DATA", "Trying to load a children modification \"%s\" that already was registered", *objectName.key().c_str());
		return false;
	}

	GameObjectData* gData = new GameObjectData();
	children[*objectName] = gData;

	if (!gData->loadData(data)) {
		LOG_ERROR("GAMEOBJECT DATA", "Adding Child \"%s\" failed", *objectName.key().c_str());
		return false;
	}
	return true;
}

void GameObjectData::setName(const std::string& gameObjectName)
{
	name = gameObjectName;
}

void GameObjectData::setTag(const std::string& gameObjectTag)
{
	tag = gameObjectTag;
}

void GameObjectData::addComponentData(const std::string& componentName, ComponentData* data)
{
	if (components.find(componentName) != components.end()) {
		LOG("GAMEOBJECT DATA: object %s component %s data has been overwritten\n", name.c_str(), componentName.c_str());
		delete components[componentName];
	}
	components[componentName] = data;
}

void GameObjectData::addChildrenData(const std::string& childrenName, GameObjectData* data)
{
	if (children.find(childrenName) != children.end()) {
		LOG("GAMEOBJECT DATA: object %s child %s data has been overwritten\n", name.c_str(), childrenName.c_str());
		delete children[childrenName];
	}
	children[childrenName] = data;
}

void GameObjectData::setBlueprint(const BlueprintData* bpRef)
{
	blueprintRef = bpRef;
}

void GameObjectData::applyChildModification(const std::string& name, GameObjectData* data)
{
	for (auto child : data->children) {
		for (auto comp : child.second->components) {
			for (auto prop : comp.second->getProperties())
				modifyChildData(child.first, comp.first, prop.first, prop.second);
		}
	}
}

void GameObjectData::applyComponentModification(const std::string& name, ComponentData* data)
{
	for (auto prop : data->getProperties())
		modifyComponentData(data->getName(), prop.first, prop.second);

}

bool GameObjectData::modifyComponentData(const std::string& componentName, const std::string& propertyName, const std::string& value)
{
	if (components.find(componentName) == components.end()) {
		LOG("GAMEOBJECT DATA: tried to modify component %s, does not exist in object %s\n", componentName.c_str(), name.c_str());
		return false;
	}
	components[componentName]->modifyProperty(propertyName, value);
	return true;
}

bool GameObjectData::modifyChildData(const std::string& childName, const std::string& componentName, const std::string& propertyName, const std::string& value)
{
	if (children.find(childName) == children.end()) {
		LOG("GAMEOBJECT DATA: tried to modify child %s, does not exist in object %s\n", childName.c_str(), name.c_str());
		return false;
	}
	return children[childName]->modifyComponentData(componentName, propertyName, value);

}

const std::string& GameObjectData::getName() const
{
	return name;
}

const std::string& GameObjectData::getTag() const
{
	return tag;
}

const std::unordered_map<std::string, ComponentData*>& GameObjectData::getComponentData() const
{
	return components;
}

const std::unordered_map<std::string, GameObjectData*>& GameObjectData::getChildrenData() const
{
	return children;
}

GameObjectData* GameObjectData::getChild(const std::string& childName, bool& exists)
{
	auto child = children.find(childName);
	if (child == children.end()) {
		exists = false;
		LOG("GAMEOBJECT DATA: child with name %s does not exist in object %s\n", childName.c_str(), name.c_str());
		return GameObjectData::empty();
	}
	exists = true;
	return  (*child).second;
}

ComponentData* GameObjectData::getComponent(const std::string& componentName, bool& exists)
{
	auto component = components.find(componentName);
	if (component == components.end()) {
		exists = false;
		LOG("GAMEOBJECT DATA: component with name %s does not exist in object %s\n", componentName.c_str(), name.c_str());
		return ComponentData::empty();
	}
	exists = true;
	return  (*component).second;
}

GameObjectData* GameObjectData::empty()
{
	GameObjectData* g = new GameObjectData();
	g->name = g->tag = "null";
	return g;
}
