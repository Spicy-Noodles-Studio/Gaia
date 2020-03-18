#include "SceneData.h"

SceneData::SceneData()
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
	cube->components["MeshRenderer"] = new ComponentData();
	cube->components["MeshRenderer"]->name = "MeshRenderer";
	cube->components["MeshRenderer"]->addProperty("mesh", "cube cube.mesh");

	s->setGameObjectData(camera);
	s->setGameObjectData(cube);

	return s;
}
