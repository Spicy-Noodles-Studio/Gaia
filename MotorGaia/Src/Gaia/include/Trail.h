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
	Ogre::BillboardSet* billboardSet;
	Ogre::RibbonTrail* trail;

	MeshRenderer* mesh;
	float length;
	Vector3 offset;
public:
	Trail(GameObject* gameObject);
	virtual ~Trail();

	void configureTrail(const std::string& trailFilename);
	void newTrail(const std::string& bone = "");

	void start();
	void stop();
	bool started() const;

	void setMeshRenderer(MeshRenderer* mesh);

	void setOffset(const Vector3& offset);
	void setLength(float length);
	void setMax(float max);
	void setColour(const Vector3& colour, float colourAlpha);
	void setColourChange(const Vector3& colourChange, float colourChangeAlpha);
	void setWidth(float width);
	void setWidthChange(float widthChange);

protected:
	virtual void handleData(ComponentData* data);

};

#endif