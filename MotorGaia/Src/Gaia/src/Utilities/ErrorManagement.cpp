#include "ErrorManagement.h"

bool notNull(void* element)
{
	if (element == nullptr) {
		LOG("ERROR: %s reference is nullptr on file %s on line %d", TO_STRING(element), __FILE__, __LINE__);
		return false;
	}
	else
		return true;
}
