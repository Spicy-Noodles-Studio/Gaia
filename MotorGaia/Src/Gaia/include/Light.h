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
private:
	Ogre::Light* light;

public:
	Light(GameObject* gameObject);
	~Light();

	enum LightType { Directional, Point, Spotlight };

	void setType(LightType type);
	void setColour(float red, float green, float blue);
	void setSpecularColour(float red, float green, float blue);

	void setDirection(const Vector3& dir);
	void setShadowsDistance(double far);

	void setVisible(bool invisible);
	bool isVisible();

	virtual void handleData(ComponentData* data);
};

#endif