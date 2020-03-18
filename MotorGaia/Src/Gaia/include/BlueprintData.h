#pragma once
#ifndef BLUEPRINT_DATA_H
#define BLUEPRINT_DATA_H

#include "Loadable.h"
#include "GameObjectData.h"

class BlueprintData : public Loadable, public GameObjectData
{
public:
	BlueprintData();
	~BlueprintData();

protected:
	virtual bool load_internal();

private:
	

};

#endif