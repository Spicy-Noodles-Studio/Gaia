#pragma once
#ifndef GAIA_COMPONENT_H
#define GAIA_COMPONENT_H

#include "Component.h"

class GAIA_API GaiaComponent : public Component
{
public:
	GaiaComponent(GameObject* gameObject);
	virtual ~GaiaComponent();
};

#endif
