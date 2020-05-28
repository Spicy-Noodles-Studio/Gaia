#include "BlueprintData.h"


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
