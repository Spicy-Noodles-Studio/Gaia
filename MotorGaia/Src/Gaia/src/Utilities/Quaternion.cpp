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

Vector3 GetForwardVector(const Quaternion& q)
{
    Vector3 forward;
    forward.x = 2 * (q.x * q.z + q.w * q.y);
    forward.y = 2 * (q.y * q.z - q.w * q.x);
    forward.z = 1 - 2 * (q.x * q.x + q.y * q.y);
    forward.normalize();
    return forward;
}

Vector3 GetUpVector(const Quaternion& q)
{
    Vector3 up;
    up.x = 2 * (q.x * q.y - q.w * q.z);
    up.y = 1 - 2 * (q.x * q.x + q.z * q.z);
    up.z = 2 * (q.y * q.z + q.w * q.x);
   up.normalize();
    return up;
}

Vector3 GetLeftVector(const Quaternion& q)
{
    Vector3 left;
    left.x = 1 - 2 * (q.y * q.y + q.z * q.z);
    left.y = 2 * (q.x * q.y + q.w * q.z);
    left.z = 2 * (q.x * q.z - q.w * q.y);
    left.normalize();
    return left;
}

Quaternion hamilton(const Quaternion& q1, const Quaternion& q2)
{
    Quaternion aux;
    aux.w = q1.w * q2.w - q1.x * q2.x - q1.y * q2.y - q1.z * q2.z;
    aux.x = q1.w * q2.x + q1.x * q2.w + q1.y * q2.z - q1.z * q2.y;
    aux.y = q1.w * q2.y - q1.x * q2.z + q1.y * q2.w + q1.z * q2.x;
    aux.z = q1.w * q2.z + q1.x * q2.y - q1.y * q2.x + q1.z * q2.w;
    return aux;
}

Quaternion Quaternion::inverse()
{
    return { w,-x,-y,-z };
}
