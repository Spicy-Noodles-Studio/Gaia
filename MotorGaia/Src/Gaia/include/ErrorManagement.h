#pragma once

#ifndef ERROR_MANAGEMENT_H
#define ERROR_MANAGEMENT_H

#include "DebugUtils.h"

//Llamar cuando demos valor a una referencia 
#define checkNull(element)\
if(element == nullptr)\
LOG("ERROR: %s reference is nullptr on file %s on line %d",#element, __FILE__, __LINE__);

#define checkNullAndBreak(element, returnValue)\
if(element == nullptr){\
LOG("ERROR: %s reference is nullptr on file %s on line %d",#element, __FILE__, __LINE__);\
return returnValue;\
}

#endif