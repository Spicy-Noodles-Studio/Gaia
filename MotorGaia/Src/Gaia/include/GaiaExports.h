#pragma once

#ifndef GAIA_EXPORTS_H
#define GAIA_EXPORTS_H

#ifdef GAIA_EXPORTS
#define GAIA_API __declspec(dllexport)
#else
#define GAIA_API __declspec(dllimport)
#endif

#endif