#pragma once
#ifndef GAIA_COMPONENT_H
#define GAIA_COMPONENT_H

#include "Component.h"

class GaiaComponent : public Component
{
public:
	GaiaComponent(GameObject* gameObject);
	~GaiaComponent();
};

#endif