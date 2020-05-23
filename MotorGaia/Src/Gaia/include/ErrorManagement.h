#pragma once

#ifndef ERROR_MANAGEMENT_H
#define ERROR_MANAGEMENT_H

#include "DebugUtils.h"
#include "GaiaExports.h"

#define TO_STRING(x) #x
//Llamar cuando demos valor a una referencia 
#define checkNull(element)\
if(element == nullptr)\
LOG("ERROR: %s reference is nullptr on file %s on line %d",#element, __FILE__, __LINE__);

#define checkNullAndBreak(element, returnValue)\
if(element == nullptr){\
LOG("ERROR: %s reference is nullptr on file %s on line %d",#element, __FILE__, __LINE__);\
return returnValue;\
}

GAIA_API bool notNullFunc(void* element, char const* name, char const* file, int line);

#define notNull(element) notNullFunc(element, #element, __FILE__, __LINE__)

#endif