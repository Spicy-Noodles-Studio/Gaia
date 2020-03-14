#pragma once
#ifndef DEBUG_UTILS_H
#define DEBUG_UTILS_H

#include <stdio.h>
#include <string>

#ifdef _DEBUG

#define LOG(message, ...)					printf((std::string(message) + std::string("\n")).c_str(), __VA_ARGS__)
#define LOG_ERROR(sender, message, ...)		printf("%s: ", sender); LOG(message, __VA_ARGS__)

#else 

#define LOG(message, ...)
#define LOG_ERROR(sender, message, ...)

#endif

#endif