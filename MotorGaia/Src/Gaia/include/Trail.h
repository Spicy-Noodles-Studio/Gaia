#pragma once
#ifndef TRAIL_H
#define TRAIL_H

#include "GaiaComponent.h"

namespace Ogre {
	class RibbonTrail;
	class BillboardSet;
}

class GAIA_API Trail : public GaiaComponent
{
private:
	Ogre::BillboardSet* bbs = nullptr;
	Ogre::RibbonTrail* trail = nullptr;

public:
	Trail(GameObject* gameObject);
	~Trail();

	void newTrail();
	void start();
	void stop();

	virtual void handleData(ComponentData* data);
};

#endif