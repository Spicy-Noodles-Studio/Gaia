#pragma once
#ifndef MATH_H
#define MATH_H
#include <GaiaExports.h>

#define _USE_MATH_DEFINES
#include <math.h>
#include <cmath>
#include <algorithm>

// Random double [0, 1] (included)
GAIA_API double random();

// Random double [from, to] (included)
GAIA_API double random(double from, double to);

// Random int [from, to) (excluded)
GAIA_API int random(int from, int to);


#define PI M_PI
#define RAD_TO_DEG (180.0 / PI)
#define DEG_TO_RAD (PI / 180.0)

#endif