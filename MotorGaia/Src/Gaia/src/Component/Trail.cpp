#include "Trail.h"

#include <OgreRibbonTrail.h>
#include <OgreSceneNode.h>
#include <OgreSceneManager.h>
#include <OgreTagPoint.h>
#include <OgreBillboardSet.h>
#include <OgreEntity.h>

#include "GameObject.h"
#include "Scene.h"
#include "ComponentRegister.h"
#include "MeshRenderer.h"
#include "GaiaData.h"

REGISTER_FACTORY(Trail);

Trail::Trail(GameObject* gameObject) : GaiaComponent(gameObject), billboardSet(nullptr), trail(nullptr), offset(0, 0, 0), length(0.0f), mesh(nullptr)
{

}

Trail::~Trail()
{
	checkNullAndBreak(gameObject);
	Scene* scene = gameObject->getScene();
	checkNullAndBreak(scene);
	Ogre::SceneManager* sceneManager = scene->getSceneManager();
	checkNullAndBreak(sceneManager);

	if (trail != nullptr) {
		Ogre::SceneNode* rootSceneNode = sceneManager->getRootSceneNode();
		checkNullAndBreak(rootSceneNode);
		rootSceneNode->detachObject(trail);
		sceneManager->destroyRibbonTrail(trail);
		trail = nullptr;
	}
	if (billboardSet != nullptr) {
		sceneManager->destroyBillboardSet(billboardSet);
		billboardSet = nullptr;
	}
}

void Trail::configureTrail(const std::string& trailFilename)
{
	GaiaData trailData;
	trailData.load(trailFilename);

	GaiaData trailParamsData = trailData.find("TrailParameters");
	for (int i = 0; i < trailParamsData.size(); i++)
	{
		if (trailParamsData[i].size() < 2) {
			LOG_ERROR("TRAIL", "Not enough arguments");
			return;
		}
		std::stringstream ss(trailParamsData[i][0].getValue());
		std::string paramName;
		double x, y, z, a;
		float value;
		std::string s;
		if (!(ss >> paramName))
		{
			LOG_ERROR("TRAIL", "Invalid trail parameter name \"%s\"", trailParamsData[i][0].getValue().c_str());
			continue;
		}

		if (paramName == "offset")
		{
			ss = std::stringstream(trailParamsData[i][1].getValue());
			if (!(ss >> x >> y >> z))
			{
				LOG_ERROR("TRAIL", "Invalid offset value \"%s\"", trailParamsData[i][1].getValue().c_str());
				continue;
			}
			setOffset(Vector3(x, y, z));
		}
		else if (paramName == "trail")
		{
			ss = std::stringstream(trailParamsData[i][1].getValue());
			if (!(ss >> s))
			{
				LOG_ERROR("TRAIL", "Invalid trail value \"%s\"", trailParamsData[i][1].getValue().c_str());
				continue;
			}

			if (s == "node")
				newTrail();
			else if (s == "bone")
			{
				if (!(ss >> s))
				{
					LOG_ERROR("TRAIL", "Invalid bone value \"%s\"", trailParamsData[i][1].getValue().c_str());
					continue;
				}
				newTrail(s);
			}
		}
		else if (paramName == "length")
		{
			ss = std::stringstream(trailParamsData[i][1].getValue());
			if (!(ss >> value))
			{
				LOG_ERROR("TRAIL", "Invalid length value \"%s\"", trailParamsData[i][1].getValue().c_str());
				continue;
			}
			setLength(value);
		}
		else if (paramName == "max")
		{
			ss = std::stringstream(trailParamsData[i][1].getValue());
			if (!(ss >> value))
			{
				LOG_ERROR("TRAIL", "Invalid max value \"%s\"", trailParamsData[i][1].getValue().c_str());
				continue;
			}
			setMax(value);
		}
		else if (paramName == "colour")
		{
			ss = std::stringstream(trailParamsData[i][1].getValue());
			if (!(ss >> x >> y >> z >> a))
			{
				LOG_ERROR("TRAIL", "Invalid colour value \"%s\"", trailParamsData[i][1].getValue().c_str());
				continue;
			}
			setColour(Vector3(x, y, z), a);
		}
		else if (paramName == "colourChange")
		{
			ss = std::stringstream(trailParamsData[i][1].getValue());
			if (!(ss >> x >> y >> z >> a))
			{
				LOG_ERROR("TRAIL", "Invalid colourChange value \"%s\"", trailParamsData[i][1].getValue().c_str());
				continue;
			}
			setColourChange(Vector3(x, y, z), a);
		}
		else if (paramName == "width")
		{
			ss = std::stringstream(trailParamsData[i][1].getValue());
			if (!(ss >> value))
			{
				LOG_ERROR("TRAIL", "Invalid width value \"%s\"", trailParamsData[i][1].getValue().c_str());
				continue;
			}
			setWidth(value);
		}
		else if (paramName == "widthChange")
		{
			ss = std::stringstream(trailParamsData[i][1].getValue());
			if (!(ss >> value))
			{
				LOG_ERROR("TRAIL", "Invalid widthChange value \"%s\"", trailParamsData[i][1].getValue().c_str());
				continue;
			}
			setWidthChange(value);
		}
		else if (paramName == "start")
		{
			ss = std::stringstream(trailParamsData[i][1].getValue());
			if (!(ss >> s))
			{
				LOG_ERROR("TRAIL", "Invalid start value \"%s\"", trailParamsData[i][1].getValue().c_str());
				continue;
			}

			if (s == "true")
				start();
			else if (s == "false")
				stop();
		}
	}
}

void Trail::newTrail(const std::string& bone)
{
	checkNullAndBreak(gameObject);
	Scene* scene = gameObject->getScene();
	checkNullAndBreak(scene);
	Ogre::SceneManager* sceneManager = scene->getSceneManager();
	checkNullAndBreak(sceneManager);
	Ogre::SceneNode* rootSceneNode = sceneManager->getRootSceneNode();
	checkNullAndBreak(rootSceneNode);

	if (trail != nullptr) {
		rootSceneNode->detachObject(trail);
		sceneManager->destroyRibbonTrail(trail);
		trail = nullptr;
	}
	if (billboardSet != nullptr) {
		sceneManager->destroyBillboardSet(billboardSet);
		billboardSet = nullptr;
	}

	// Create billboard set
	billboardSet = sceneManager->createBillboardSet(gameObject->getName() + "BB");
	billboardSet->setDefaultDimensions(0.25, 0.25);
	billboardSet->setMaterialName("Billboard");
	billboardSet->createBillboard(0, 0, 0, Ogre::ColourValue::Black);

	// Create trail
	trail = sceneManager->createRibbonTrail(gameObject->getName() + "Trail");
	trail->setMaterialName("Trail");

	if (bone == "") // Node trail
	{
		checkNullAndBreak(gameObject->node);
		trail->addNode(gameObject->node);
	}
	else // Bone trail
	{
		checkNullAndBreak(mesh);
		Ogre::Entity* entity = mesh->getMesh(mesh->getMeshId());
		checkNullAndBreak(entity);
		checkNullAndBreak(billboardSet);

		Ogre::TagPoint* tp = entity->attachObjectToBone(bone, billboardSet, Ogre::Quaternion::IDENTITY,
			Ogre::Vector3(offset.x, offset.y, offset.z));
		trail->addNode(tp);
	}

	rootSceneNode->attachObject(trail);
}

void Trail::start()
{
	checkNullAndBreak(trail);
	/*trail->(length);
	LOG("LENGTH: %d\n", length);*/
	trail->setVisible(true); // esto se puede mejorar!
}

void Trail::stop()
{
	checkNullAndBreak(trail);

	//trail->setTrailLength(0);
	trail->setVisible(false); // esto se puede mejorar!
}

bool Trail::started() const
{
	checkNullAndBreak(trail, false);

	return /*trail->getTrailLength();*/trail->isVisible(); // esto se puede mejorar!
}

void Trail::handleData(ComponentData* data)
{
	checkNullAndBreak(data);

	float length = 20; float max = 40; Vector3 colour = Vector3(1, 1, 1); float colourAlpha = 1.0;
	Vector3 colourChange = Vector3(0.5, 0.5, 0.5); float colourChangeAlpha = 0.5;
	float width = 0.5; float widthChange = 0.1;

	for (auto prop : data->getProperties()) {
		std::stringstream ss(prop.second);

		if (prop.first == "offset") {
			setVector3(offset);
		}
		else if (prop.first == "trail") {

			std::string type;
			if (ss >> type) {
				if (type == "node")
					newTrail("");
				else if (type == "bone")
				{
					std::string bone;
					ss >> bone;
					newTrail(bone);
				}
			}
			else {
				LOG_ERROR("TRAIL", "Invalid data format. Property \"trail\"");
			}
		}
		else if (prop.first == "start") {
			if (prop.second == "true") {
				start();
			}
			else if (prop.second == "false") {
				stop();
			}
			else {
				LOG_ERROR("TRAIL", "Invalid value \"%s\". Property \"start\"", prop.second.c_str());
			}
		}
		else if (prop.first == "length") {
			setFloat(length);
		}
		else if (prop.first == "max") {
			setFloat(max);
		}
		else if (prop.first == "colour") {
			setVector3(colour);
		}
		else if (prop.first == "alpha") {
			setFloat(colourAlpha);
		}
		else if (prop.first == "colourChange") {
			setVector3(colourChange);
		}
		else if (prop.first == "alphaChange") {
			setFloat(colourChangeAlpha);
		}
		else if (prop.first == "width") {
			setFloat(width);
		}
		else if (prop.first == "widthChange") {
			setFloat(widthChange);
		}
		else {
			LOG_ERROR("TRAIL", "Invalid property name \"%s\"", prop.first.c_str());
		}
	}

	if (trail != nullptr)
	{
		setLength(length);
		setMax(max);
		setColour(colour, colourAlpha);
		setColourChange(colourChange, colourChangeAlpha);
		setWidth(width);
		setWidthChange(widthChange);
	}
}

void Trail::setMeshRenderer(MeshRenderer* mesh)
{
	this->mesh = mesh;
}

void Trail::setOffset(const Vector3& offset)
{
	this->offset = offset;
}

void Trail::setLength(float length)
{
	checkNullAndBreak(trail);

	this->length = length;
	trail->setTrailLength(length);
}

void Trail::setMax(float max)
{
	checkNullAndBreak(trail);

	trail->setMaxChainElements(max);
}

void Trail::setColour(const Vector3& colour, float colourAlpha)
{
	checkNullAndBreak(trail);

	trail->setInitialColour(0, colour.x, colour.y, colour.z, colourAlpha);
}

void Trail::setColourChange(const Vector3& colourChange, float colourChangeAlpha)
{
	checkNullAndBreak(trail);

	trail->setColourChange(0, colourChange.x, colourChange.y, colourChange.z, colourChangeAlpha);
}

void Trail::setWidth(float width)
{
	checkNullAndBreak(trail);

	trail->setInitialWidth(0, width);
}

void Trail::setWidthChange(float widthChange)
{
	checkNullAndBreak(trail);

	trail->setWidthChange(0, widthChange);
}
