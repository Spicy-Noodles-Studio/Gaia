#include "Quaternion.h"

Quaternion ToQuaternion(double yaw, double pitch, double roll) // yaw (Z), pitch (Y), roll (X)
{
 
    double _yaw = yaw * 3.1415 / 180;
    double _pitch = pitch * 3.1415 / 180;
    double _roll = roll * 3.1415 / 180;

    // Abbreviations for the various angular functions
    double cy = cos(_yaw * 0.5);
    double sy = sin(_yaw * 0.5);
    double cp = cos(_pitch * 0.5);
    double sp = sin(_pitch * 0.5);
    double cr = cos(_roll * 0.5);
    double sr = sin(_roll * 0.5);
    

    Quaternion q;
    q.w = cy * cp * cr + sy * sp * sr;
    q.x = cy * cp * sr - sy * sp * cr;
    q.y = sy * cp * sr + cy * sp * cr;
    q.z = sy * cp * cr - cy * sp * sr;

    return q;
}

Vector3 GetForwardVector(Quaternion q)
{
    Vector3 forward;
    forward.x = 2 * (q.x * q.z + q.w * q.y);
    forward.y = 2 * (q.y * q.z - q.w * q.x);
    forward.z = 1 - 2 * (q.x * q.x + q.y * q.y);
    forward.normalize();
    return forward;
}

Vector3 GetUpVector(Quaternion q)
{
    Vector3 up;
    up.x = 2 * (q.x * q.y - q.w * q.z);
    up.y = 1 - 2 * (q.x * q.x + q.z * q.z);
    up.z = 2 * (q.y * q.z + q.w * q.x);
   up.normalize();
    return up;
}

Vector3 GetLeftVector(Quaternion q)
{
    Vector3 left;
    left.x = 1 - 2 * (q.y * q.y + q.z * q.z);
    left.y = 2 * (q.x * q.y + q.w * q.z);
    left.z = 2 * (q.x * q.z - q.w * q.y);
    left.normalize();
    return left;
}
