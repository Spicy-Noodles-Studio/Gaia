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
	camera->components["Transform"] = new ComponentData();
	camera->components["Transform"]->name = "Transform";
	camera->components["Transform"]->addProperty("position", "0 0 400");
	camera->components["Camera"] = new ComponentData();
	camera->components["Camera"]->name = "Camera";
	camera->components["Camera"]->addProperty("main", "true");
	camera->components["Light"] = new ComponentData();
	camera->components["Light"]->name = "Light";
	camera->components["Light"]->addProperty("colour", "1.0 1.0 1.0");
	camera->components["Light"]->addProperty("type", "Point");
	// Cube with tranform
	GameObjectData* cube = new GameObjectData();
	cube->name = "Cube";
	cube->components["Transform"] = new ComponentData();
	cube->components["Transform"]->name = "Transform";
	cube->components["Transform"]->addProperty("position", "0 0 0");
	cube->components["Transform"]->addProperty("rotation", "45 45 45");
	cube->components["MeshRenderer"] = new ComponentData();
	cube->components["MeshRenderer"]->name = "MeshRenderer";
	cube->components["MeshRenderer"]->addProperty("mesh", "cube cube.mesh");

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
