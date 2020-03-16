#pragma once
#ifndef QUATERNION_H
#define QUATERNION_H

#include "Vector3.h"
#include "GaiaExports.h"

class GAIA_API Quaternion
{
public:
    double x, y, z, w;

public:
    Quaternion();
    Quaternion(const Vector3& euler);
    Quaternion(double xEuler, double yEuler, double zEuler);
    Quaternion(double x, double y, double z, double w);
    ~Quaternion();

    bool operator==(const Quaternion& other) const;
    bool operator!=(const Quaternion& other) const;
    Quaternion& operator=(const Quaternion& other);
    Quaternion operator+(const Quaternion& other) const;
    Quaternion operator*(const Quaternion& other) const;
    Quaternion operator*(double n) const;
    Quaternion& operator*=(double n);
    Quaternion& operator*=(const Quaternion& other);

    double dot(const Quaternion& other) const;

    Quaternion& set(double x, double y, double z, double w);
    Quaternion& set(double xEuler, double yEuler, double zEuler);
    Quaternion& set(const Vector3& euler);
    Quaternion& set(const Quaternion& quat);
    
    Quaternion& fromAngleAxis(double angle, const Vector3& axis);
    Quaternion& lerp(Quaternion q1, Quaternion q2, double t);
    Quaternion& slerp(Quaternion q1, Quaternion q2, double t, double threshold = 0.05);

    Quaternion& identity();
    Quaternion& inverse();
    Quaternion& normalize();

    Quaternion getIdentity();
    Quaternion getInverse();
    Quaternion getNormalize();

    Quaternion& rotationFromTo(const Vector3& from, const Vector3& to);
    Vector3 toEuler();
    void toAngleAxis(double& angle, Vector3& axis);

    static Quaternion AnglesToQuaternion(double yaw, double pitch, double roll);
    static Quaternion hamilton(const Quaternion& q1, const Quaternion& q2);
};

GAIA_API Vector3 GetForwardVector(const Quaternion& q);
GAIA_API Vector3 GetUpVector(const Quaternion& q);
GAIA_API Vector3 GetLeftVector(const Quaternion& q);

#endif