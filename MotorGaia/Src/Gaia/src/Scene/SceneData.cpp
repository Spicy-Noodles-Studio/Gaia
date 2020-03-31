#include "SceneData.h"
#include "DebugUtils.h"

SceneData::SceneData() : Loadable()
{
}

SceneData::~SceneData()
{
	for (int i = 0; i < gameObjectData.size(); i++) {
		delete gameObjectData[i];
		gameObjectData[i] = nullptr;
	}
	gameObjectData.clear();
}

void SceneData::setName(const std::string& sceneName)
{
	name = sceneName;
}

void SceneData::setGameObjectData(GameObjectData* data)
{
	gameObjectData.push_back(data);
}

const std::string& SceneData::getName() const
{
	return name;
}

const std::vector<GameObjectData*>& SceneData::getGameObjectsData() const
{
	return gameObjectData;
}

SceneData* SceneData::empty()
{
	SceneData* s = new SceneData();
	s->name = "Default";
	
	// Camera with white light
	GameObjectData* camera = new GameObjectData();
	camera->name = "MainCamera";
	//Transform
	camera->components.push_back(new ComponentData());
	camera->componentsIndexes["Transform"] = camera->components.size() - 1;
	camera->components[0]->name = "Transform";
	camera->components[0]->addProperty("position", "0 0 400");
	//Camera
	camera->components.push_back(new ComponentData());
	camera->componentsIndexes["Camera"] = camera->components.size() - 1;
	camera->components[1]->name = "Camera";
	camera->components[1]->addProperty("main", "true");
	//Light
	camera->components.push_back(new ComponentData());
	camera->componentsIndexes["Light"] = camera->components.size() - 1;
	camera->components[2]->name = "Light";
	camera->components[2]->addProperty("colour", "1.0 1.0 1.0");
	camera->components[2]->addProperty("type", "Point");
	// Cube with tranform
	GameObjectData* cube = new GameObjectData();
	cube->name = "Cube";
	//Transform
	cube->components.push_back(new ComponentData());
	cube->componentsIndexes["Transform"] = cube->components.size() - 1;
	cube->components[0]->name = "Transform";
	cube->components[0]->addProperty("position", "0 0 0");
	cube->components[0]->addProperty("rotation", "45 45 45");
	//Mesh renderer
	cube->components.push_back(new ComponentData());
	cube->componentsIndexes["MeshRenderer"] = cube->components.size() - 1;
	cube->components[1]->name = "MeshRenderer";
	cube->components[1]->addProperty("mesh", "Cube Cube.mesh");

	s->setGameObjectData(camera);
	s->setGameObjectData(cube);

	return s;
}

bool SceneData::load_internal()
{
	nlohmann::json::const_iterator name = fileData.find("SceneName");
	if (name == fileData.end()) {
		LOG("DATA LOADER: SceneName not found\n");
		return false;
	}
	setName(*name);

	nlohmann::json::const_iterator objects = fileData.find("SceneObjects");
	if (objects != fileData.end()) {
		for (auto& gameObject : (*objects).items()) {
			GameObjectData* gameObjectData = new GameObjectData();
			if (!gameObjectData->loadData(gameObject.value())) {
				LOG_ERROR("SCENE DATA", "Error ocurred trying to load \"%s\" GameObject in Scene \"%s\"", gameObjectData->name.c_str(), this->name.c_str());
				return false;
			}
			setGameObjectData(gameObjectData);
		}
	}

	return true;
}
