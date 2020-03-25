#include "BlueprintData.h"
#include "DebugUtils.h"
#include "ResourcesManager.h"

BlueprintData::BlueprintData() : Loadable()
{
}

BlueprintData::~BlueprintData()
{

}

bool BlueprintData::load_internal()
{
	return loadData(fileData);
}
