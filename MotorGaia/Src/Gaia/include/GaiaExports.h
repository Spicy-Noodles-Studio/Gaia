#pragma once

#ifndef GAIA_EXPORTS_H
#define GAIA_EXPORTS_H

#ifdef USEDLL
#ifdef GAIA_EXPORTS
#define GAIA_API __declspec(dllexport)
#else
#define GAIA_API __declspec(dllimport)
#endif
#else 
#define GAIA_API

#endif

#endif