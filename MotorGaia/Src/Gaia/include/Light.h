#pragma once
#ifndef LIGHT_H
#define LIGHT_H

#include "GaiaComponent.h"
#include "Vector3.h"

namespace Ogre {
	class Light;
}

class GAIA_API Light : public GaiaComponent
{
public:
	enum LightType { Directional, Point, Spotlight };

private:
	Ogre::Light* light;
	float intensity;

public:
	Light(GameObject* gameObject);
	virtual ~Light();

	void setType(LightType type);
	void setColour(float red, float green, float blue);
	void setSpecularColour(float red, float green, float blue);

	void setShadowsDistance(double far);

	void setVisible(bool invisible);
	bool isVisible() const;

	void setIntensity(float intensity);
	float getIntensity() const;

protected:
	virtual void handleData(ComponentData* data);
};
#endif