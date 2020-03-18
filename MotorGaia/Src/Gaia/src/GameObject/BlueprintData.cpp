#include "BlueprintData.h"
#include "DebugUtils.h"
#include "ResourcesManager.h"

BlueprintData::BlueprintData() : Loadable(this)
{
}

BlueprintData::~BlueprintData()
{
}

bool BlueprintData::load_internal()
{
	return loadData(fileData);
}
