#pragma once
#ifndef TRAIL_H
#define TRAIL_H

#include "GaiaComponent.h"
#include "Vector3.h"

namespace Ogre {
	class RibbonTrail;
	class BillboardSet;
}

class MeshRenderer;

class GAIA_API Trail : public GaiaComponent
{
private:
	Ogre::BillboardSet* bbs;
	Ogre::RibbonTrail* trail;

	Vector3 offset;

public:
	Trail(GameObject* gameObject);
	~Trail();

	void newTrail(const std::string& bone, MeshRenderer* mesh = nullptr);
	void start();
	void stop();

	virtual void handleData(ComponentData* data);

	void setOffset(const Vector3& offset);
	void setLength(float length);
	void setMax(float max);
	void setColour(const Vector3& colour, float colourAlpha);
	void setColourChange(const Vector3& colourChange, float colourChangeAlpha);
	void setWidth(float width);
	void setWidthChange(float widthChange);
};

#endif