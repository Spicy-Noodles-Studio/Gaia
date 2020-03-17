#pragma once

#ifndef STRIDER_H
#define STRIDER_H

#include "GaiaComponent.h"

class Strider : public GaiaComponent
{
public:
	Strider(GameObject* gameObject);

	virtual void handleData(ComponentData* data);
	void stride(const std::string& mesh);
};
#endif
