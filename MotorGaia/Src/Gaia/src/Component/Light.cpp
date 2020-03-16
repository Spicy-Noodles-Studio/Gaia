#include "Light.h"
#include "GameObject.h"
#include "Scene.h"
#include "ComponentData.h"
#include <OgreSceneManager.h>
#include <sstream>

Light::Light(GameObject* gameObject) : GaiaComponent(gameObject)
{
	light = gameObject->getScene()->getSceneManager()->createLight(gameObject->getName() + " -L");
	gameObject->node->attachObject(light);
}

Light::~Light()
{
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
	light->setDiffuseColour(red, green, blue);
}

void Light::setSpecularColour(float red, float green, float blue)
{
	light->setSpecularColour(red, green, blue);
}

void Light::setDirection(const Vector3& dir)
{
	gameObject->node->setDirection(Ogre::Vector3(dir.x, dir.y, dir.z));
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

void Light::handleData(ComponentData* data)
{
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
		else if (prop.first == "colour") {
			double x, y, z; ss >> x >> y >> z;
			setColour(x, y, z);
		}
	}
}
