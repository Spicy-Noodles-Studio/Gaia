#pragma once
#ifndef MACROS_H
#define MACROS_H

// HANDLE DATA MACROS

#define setInt(variable)                                       \
	if (!(ss >> variable)){                                                     \
		LOG("setInt: Invalid property with name \"%s\"", prop.first.c_str());     \
	}                                                                               \

#define setFloat(variable)                                    \
	if (!(ss >> variable)){                                                     \
		LOG("setFloat: Invalid property with name \"%s\"", prop.first.c_str());     \
	}                                                                               \

#define setVector3(variable)                                  \
	if (!(ss >> variable.x >> variable.y >> variable.z)){                       \
		LOG("setVector3: Invalid property with name \"%s\"", prop.first.c_str());   \
	}                                                                               \

#define setString(variable)                                   \
	if (!(ss >> variable)){                                                     \
		LOG("setString: Invalid property with name \"%s\"", prop.first.c_str());    \
	}                                                                               \

#define setBool(variable) \
	if (!(ss >> variable)){ \
		LOG("setBool: Invalid property with name \"%s\"", prop.first.c_str()); \
	}

#endif


