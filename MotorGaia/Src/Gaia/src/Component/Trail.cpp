#include "Trail.h"

#include <OgreRibbonTrail.h>
#include <OgreSceneNode.h>
#include <OgreSceneManager.h>
#include <OgreTagPoint.h>
#include <OgreBillboardSet.h>

#include "GameObject.h"
#include "Scene.h"
#include "ComponentRegister.h"

REGISTER_FACTORY(Trail);

Trail::Trail(GameObject* gameObject) : GaiaComponent(gameObject), trail(nullptr)
{
	
}

Trail::~Trail()
{
	if (trail != nullptr) {
		gameObject->getScene()->getSceneManager()->getRootSceneNode()->detachObject(trail);
		gameObject->getScene()->getSceneManager()->destroyRibbonTrail(trail);
		gameObject->getScene()->getSceneManager()->destroyBillboardSet(bbs);
		trail = nullptr;
		bbs = nullptr;
	}
}

void Trail::newTrail()
{
	if (trail != nullptr) {
		gameObject->getScene()->getSceneManager()->getRootSceneNode()->detachObject(trail);
		gameObject->getScene()->getSceneManager()->destroyRibbonTrail(trail);
		gameObject->getScene()->getSceneManager()->destroyBillboardSet(bbs);
		trail = nullptr;
		bbs = nullptr;
	}

	// Create billboard
	bbs = gameObject->getScene()->getSceneManager()->createBillboardSet(gameObject->getName() + "BB");
	bbs->setDefaultDimensions(0.25, 0.25);
	bbs->setMaterialName("Billboard");
	bbs->createBillboard(0, 0, 0, Ogre::ColourValue::Black);

	//Ogre::TagPoint* tp = gameObject->node->attachObjectToBone("joint5", bbs);
	
	// Create trail
	trail = gameObject->getScene()->getSceneManager()->createRibbonTrail(gameObject->getName() + "Trail");
	trail->setMaterialName("Trail");

	trail->setTrailLength(20);
	trail->setMaxChainElements(40);
	trail->setInitialColour(0, 1.0, 0.0, 0.0);
	trail->setColourChange(0, 0.5, 0.5, 0.5, 0.5);
	trail->setInitialWidth(0, 0.5);
	trail->setWidthChange(0, 0.1);
	// Node trail
	trail->addNode(gameObject->node);

	gameObject->getScene()->getSceneManager()->getRootSceneNode()->attachObject(trail);
}

void Trail::start()
{
	if (trail != nullptr)
		trail->setVisible(true); // esto se puede mejorar!
	else
		LOG("TRAIL: trying to start a NULL trail\n");
}

void Trail::stop()
{
	if (trail != nullptr)
		trail->setVisible(false); // esto se puede mejorar!
	else
		LOG("TRAIL: trying to stop a NULL trail\n");
}

void Trail::handleData(ComponentData* data)
{
	float length = 20; float max = 40; Vector3 colour = Vector3(1, 1, 1); float colourAlpha = 1.0;
	Vector3 colourChange = Vector3(0.5, 0.5, 0.5); float colourChangeAlpha = 0.5;
	float width = 0.5; float widthChange = 0.1;

	for (auto prop : data->getProperties()) {
		std::stringstream ss(prop.second);
		
		if (prop.first == "trail") {
			
			std::string type;
			if (ss >> type) {
				newTrail();
			}
			else {
				LOG("TRAIL: invalid data format. Property \"trail\"\n");
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
				LOG("TRAIL: invalid value \"%s\". Property \"start\"\n", prop.second.c_str());
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
			LOG("TRAIL: invalid property name \"%s\"\n", prop.first.c_str());
		}
	}

	trail->setTrailLength(length);
	trail->setMaxChainElements(max);
	trail->setInitialColour(0, colour.x, colour.y, colour.z, colourAlpha);
	trail->setColourChange(0, colourChange.x, colourChange.y, colourChange.z, colourChangeAlpha);
	trail->setInitialWidth(0, width);
	trail->setWidthChange(0, widthChange);
}
