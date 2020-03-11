#pragma once
#ifndef QUATERNION_H
#define QUATERNION_H

#include <math.h>
#include "Vector3.h"
struct Quaternion
{
    double w, x, y, z;
};

Quaternion ToQuaternion(double yaw, double pitch, double roll);

Vector3 GetForwardVector(Quaternion q);
Vector3 GetUpVector(Quaternion q);
Vector3 GetLeftVector(Quaternion q);

#endif