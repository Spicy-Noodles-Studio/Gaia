#include "ErrorManagement.h"

bool notNullFunc(void* element, char const* name, char const* file, int line)
{
	if (element == nullptr) {
		LOG("ERROR: %s reference is nullptr on file %s on line %d", name, file, line);
		return false;
	}
	else
		return true;
}
