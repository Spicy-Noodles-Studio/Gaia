#pragma once
#ifndef QUATERNION_H
#define QUATERNION_H

#include <math.h>
#include "Vector3.h"
#include "GaiaExports.h"

struct GAIA_API Quaternion
{
    double w, x, y, z;

    Quaternion inverse();
};

GAIA_API Quaternion ToQuaternion(double yaw, double pitch, double roll);
GAIA_API Quaternion hamilton(const Quaternion& q1, const Quaternion& q2);

GAIA_API Vector3 GetForwardVector(const Quaternion& q);
GAIA_API Vector3 GetUpVector(const Quaternion& q);
GAIA_API Vector3 GetLeftVector(const Quaternion& q);

#endif