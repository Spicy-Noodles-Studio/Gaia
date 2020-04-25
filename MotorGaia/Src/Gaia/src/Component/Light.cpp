#include "Light.h"

#include <sstream>
#include <OgreSceneManager.h>

#include "GameObject.h"
#include "Scene.h"
#include "ComponentData.h"
#include "ComponentRegister.h"

REGISTER_FACTORY(Light);

Light::Light(GameObject* gameObject) : GaiaComponent(gameObject), intensity(1.0f)
{
	light = gameObject->getScene()->getSceneManager()->createLight(gameObject->node->getName() + " -L");
	gameObject->node->attachObject(light);
}

Light::~Light()
{
	gameObject->node->detachObject(light);
	gameObject->getScene()->getSceneManager()->destroyLight(light);
	light = nullptr;
}

/*
-Directional-
-Point-
-SpotLight-
*/
void Light::setType(LightType type)
{
	switch (type)
	{
	case Directional:
		light->setType(Ogre::Light::LT_DIRECTIONAL);
		break;
	case Point:
		light->setType(Ogre::Light::LT_POINT);
		break;
	case Spotlight:
		light->setType(Ogre::Light::LT_SPOTLIGHT);
		break;
	default:
		break;
	}
}

void Light::setColour(float red, float green, float blue)
{
	light->setDiffuseColour(red * intensity, green * intensity, blue * intensity);
}

void Light::setSpecularColour(float red, float green, float blue)
{
	light->setSpecularColour(red, green, blue);
}

/*
--Distancia,desde la camara, a la que las sombras dejan de renderizarse--
*/
void Light::setShadowsDistance(double far)
{
	light->setShadowFarDistance(far);
}

/*
--Al ser luces, el no ser visible significa que no afectan a los objetos en la escena--
*/
void Light::setVisible(bool visible)
{
	light->setVisible(visible);
}
/*
--Al ser luces, el no ser visible significa que no afectan a los objetos en la escena--
*/
bool Light::isVisible()
{
	return light->isVisible();
}

void Light::setIntensity(float intensity)
{
	this->intensity = intensity;
}

float Light::getIntensity()
{
	return intensity;
}

void Light::handleData(ComponentData* data)
{
	float intensity = 1.0f; Vector3 colour = Vector3(1, 1, 1); bool visible = true; float shadowDistance = 10.0f;

	for (auto prop : data->getProperties()) {
		std::stringstream ss(prop.second);

		if (prop.first == "type") {
			if (prop.second == "Point")
				setType(Point);
			else if (prop.second == "Spotlight")
				setType(Spotlight);
			else if (prop.second == "Directional")
				setType(Directional);
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
