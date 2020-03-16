#include "SceneData.h"

SceneData::SceneData()
{
}

SceneData::~SceneData()
{
	for (int i = 0; i < gData.size(); i++) {
		delete gData[i];
		gData[i] = nullptr;
	}
	gData.clear();
}

void SceneData::setName(const std::string& sceneName)
{
	name = sceneName;
}

void SceneData::setGameObjectData(GameObjectData* data)
{
	gData.push_back(data);
}

const std::string& SceneData::getName() const
{
	return name;
}

const std::vector<GameObjectData*>& SceneData::getGameObjectsData() const
{
	return gData;
}

SceneData* SceneData::empty()
{
	SceneData* s = new SceneData();
	s->name = "Default";
	
	// Camera with white light
	GameObjectData* camera = new GameObjectData();
	camera->name = "MainCamera";
	camera->componentData["Transform"] = new ComponentData();
	camera->componentData["Transform"]->name = "Transform";
	camera->componentData["Transform"]->addProperty("position", "0 0 400");
	camera->componentData["Camera"] = new ComponentData();
	camera->componentData["Camera"]->name = "Camera";
	camera->componentData["Camera"]->addProperty("main", "true");
	camera->componentData["Light"] = new ComponentData();
	camera->componentData["Light"]->name = "Light";
	camera->componentData["Light"]->addProperty("colour", "1.0 1.0 1.0");
	camera->componentData["Light"]->addProperty("type", "Point");
	// Cube with tranform
	GameObjectData* cube = new GameObjectData();
	cube->name = "Cube";
	cube->componentData["Transform"] = new ComponentData();
	cube->componentData["Transform"]->name = "Transform";
	cube->componentData["Transform"]->addProperty("position", "0 0 0");
	cube->componentData["MeshRenderer"] = new ComponentData();
	cube->componentData["MeshRenderer"]->name = "MeshRenderer";
	cube->componentData["MeshRenderer"]->addProperty("mesh", "cube cube.mesh");

	s->setGameObjectData(camera);
	s->setGameObjectData(cube);

	return s;
}
