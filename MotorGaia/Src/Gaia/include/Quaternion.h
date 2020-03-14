#pragma once
#ifndef QUATERNION_H
#define QUATERNION_H

#include <math.h>
#include "Vector3.h"
struct Quaternion
{
    double w, x, y, z;

    Quaternion inverse();
};

Quaternion ToQuaternion(double yaw, double pitch, double roll);
Quaternion hamilton(const Quaternion& q1, const Quaternion& q2);

Vector3 GetForwardVector(const Quaternion& q);
Vector3 GetUpVector(const Quaternion& q);
Vector3 GetLeftVector(const Quaternion& q);

#endif