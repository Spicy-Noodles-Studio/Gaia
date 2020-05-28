#include "Light.h"

#include <sstream>
#include <OgreSceneManager.h>

#include "GameObject.h"
#include "Scene.h"
#include "ComponentData.h"
#include "ComponentRegister.h"

REGISTER_FACTORY(Light);

Light::Light(GameObject* gameObject) : GaiaComponent(gameObject), light(nullptr), intensity(1.0f)
{
	checkNullAndBreak(gameObject);
	Scene* scene = gameObject->getScene();
	checkNullAndBreak(scene);
	Ogre::SceneManager* sceneManager = scene->getSceneManager();
	checkNullAndBreak(sceneManager);
	checkNullAndBreak(gameObject->node);
	light = sceneManager->createLight(gameObject->node->getName() + " -L");
	checkNullAndBreak(light);
	gameObject->node->attachObject(light);
}

Light::~Light()
{
	checkNullAndBreak(gameObject);
	checkNullAndBreak(gameObject->node);
	checkNullAndBreak(light);

	gameObject->node->detachObject(light);

	Scene* scene = gameObject->getScene();
	checkNullAndBreak(scene);

	Ogre::SceneManager* sceneManager = scene->getSceneManager();
	checkNullAndBreak(sceneManager);

	sceneManager->destroyLight(light);
	light = nullptr;
}

/*
-Directional-
-Point-
-SpotLight-
*/
void Light::setType(LightType type)
{
	checkNullAndBreak(light);

	switch (type)
	{
	case LightType::Directional:
		light->setType(Ogre::Light::LT_DIRECTIONAL);
		break;
	case LightType::Point:
		light->setType(Ogre::Light::LT_POINT);
		break;
	case LightType::Spotlight:
		light->setType(Ogre::Light::LT_SPOTLIGHT);
		break;
	default:
		break;
	}
}

void Light::setColour(float red, float green, float blue)
{
	checkNullAndBreak(light);

	light->setDiffuseColour(red * intensity, green * intensity, blue * intensity);
}

void Light::setSpecularColour(float red, float green, float blue)
{
	checkNullAndBreak(light);

	light->setSpecularColour(red, green, blue);
}

/*
--Distancia,desde la camara, a la que las sombras dejan de renderizarse--
*/
void Light::setShadowsDistance(double far)
{
	checkNullAndBreak(light);

	light->setShadowFarDistance(far);
}

/*
--Al ser luces, el no ser visible significa que no afectan a los objetos en la escena--
*/
void Light::setVisible(bool visible)
{
	checkNullAndBreak(light);

	light->setVisible(visible);
}
/*
--Al ser luces, el no ser visible significa que no afectan a los objetos en la escena--
*/
bool Light::isVisible() const
{
	checkNullAndBreak(light, false);

	return light->isVisible();
}

void Light::setIntensity(float intensity)
{
	this->intensity = intensity;
}

float Light::getIntensity() const
{
	return intensity;
}

void Light::handleData(ComponentData* data)
{
	checkNullAndBreak(data);

	float intensity = 1.0f; Vector3 colour = Vector3(1, 1, 1); bool visible = true; float shadowDistance = 10.0f;

	for (auto prop : data->getProperties()) {
		std::stringstream ss(prop.second);

		if (prop.first == "type") {
			if (prop.second == "Point")
				setType(LightType::Point);
			else if (prop.second == "Spotlight")
				setType(LightType::Spotlight);
			else if (prop.second == "Directional")
				setType(LightType::Directional);
			else
				LOG("LIGHT: %s not valid light type\n", prop.second.c_str());
		}
		else if (prop.first == "intensity") {
			setFloat(intensity);
		}
		else if (prop.first == "colour") {
			setVector3(colour)
		}
		else if (prop.first == "visible") {
			setBool(visible)
		}
		else if (prop.first == "shadowDistance") {
			setFloat(shadowDistance);
		}
	}

	setIntensity(intensity);
	setColour(colour.x, colour.y, colour.z);
	setVisible(visible);
	setShadowsDistance(shadowDistance);
}
