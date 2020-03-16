#include "Quaternion.h"
#include "MathUtils.h"

Quaternion::Quaternion() : x(0.0), y(0.0), z(0.0), w(1.0)
{

}

Quaternion::Quaternion(double x, double y, double z, double w) : x(x), y(y), z(z), w(w)
{

}

Quaternion::Quaternion(const Vector3& euler)
{
	set(euler.x, euler.y, euler.z);
}

Quaternion::Quaternion(double xEuler, double yEuler, double zEuler)
{
	set(xEuler, yEuler, zEuler);
}

Quaternion::~Quaternion()
{

}

bool Quaternion::operator==(const Quaternion& other) const
{
	return ((x == other.x) &&
			(y == other.y) &&
			(z == other.z) &&
			(w == other.w));
}

bool Quaternion::operator!=(const Quaternion& other) const
{
	return !(*this == other);
}

Quaternion& Quaternion::operator=(const Quaternion& other)
{
	x = other.x;
	y = other.y;
	z = other.z;
	w = other.w;
	return *this;
}

Quaternion Quaternion::operator*(const Quaternion& other) const
{
	Quaternion tmp;

	tmp.w = (other.w * w) - (other.x * x) - (other.y * y) - (other.z * z);
	tmp.x = (other.w * x) + (other.x * w) + (other.y * z) - (other.z * y);
	tmp.y = (other.w * y) + (other.y * w) + (other.z * x) - (other.x * z);
	tmp.z = (other.w * z) + (other.z * w) + (other.x * y) - (other.y * x);

	return tmp;
}

Quaternion Quaternion::operator*(double n) const
{
	return Quaternion(n * x, n * y, n * z, n * w);
}

Quaternion& Quaternion::operator*=(double n)
{
	x *= n;
	y *= n;
	z *= n;
	w *= n;
	return *this;
}

Quaternion& Quaternion::operator*=(const Quaternion& other)
{
	return (*this = other * (*this));
}

Quaternion Quaternion::operator+(const Quaternion& b) const
{
	return Quaternion(x + b.x, y + b.y, z + b.z, w + b.w);
}

double Quaternion::dot(const Quaternion& q2) const
{
	return (x * q2.x) + (y * q2.y) + (z * q2.z) + (w * q2.w);
}

Quaternion& Quaternion::set(double x, double y, double z, double w)
{
	this->x = x;
	this->y = y;
	this->z = z;
	this->w = w;
	return *this;
}

Quaternion& Quaternion::set(double xEuler, double yEuler, double zEuler)
{
	double angle;

	angle = xEuler * 0.5;
	const double sr = sin(angle);
	const double cr = cos(angle);

	angle = yEuler * 0.5;
	const double sp = sin(angle);
	const double cp = cos(angle);

	angle = zEuler * 0.5;
	const double sy = sin(angle);
	const double cy = cos(angle);

	const double cpcy = cp * cy;
	const double spcy = sp * cy;
	const double cpsy = cp * sy;
	const double spsy = sp * sy;

	x = (double)(sr * cpcy - cr * spsy);
	y = (double)(cr * spcy + sr * cpsy);
	z = (double)(cr * cpsy - sr * spcy);
	w = (double)(cr * cpcy + sr * spsy);

	return normalize();
}

Quaternion& Quaternion::set(const Vector3& euler)
{
	return set(euler.x, euler.y, euler.z);
}

Quaternion& Quaternion::set(const Quaternion& quat)
{
	return (*this = quat);
}

Quaternion& Quaternion::fromAngleAxis(double angle, const Vector3& axis)
{
	const double halfAngle = 0.5f * angle;
	const double hSin = sin(halfAngle);
	w = cos(halfAngle);
	x = hSin * axis.x;
	y = hSin * axis.y;
	z = hSin * axis.z;
	return *this;
}

Quaternion& Quaternion::lerp(Quaternion q1, Quaternion q2, double t)
{
	const double scale = 1.0f - t;
	return (*this = (q1 * scale) + (q2 * t));
}

Quaternion& Quaternion::slerp(Quaternion q1, Quaternion q2, double t, double threshold)
{
	double angle = q1.dot(q2);

	// make sure we use the short rotation
	if (angle < 0.0f)
	{
		q1 *= -1.0f;
		angle *= -1.0f;
	}

	if (angle <= (1 - threshold)) // spherical interpolation
	{
		const double theta = acos(angle);
		const double invsintheta = 1.0 / sqrt((sin(theta)));
		const double scale = sin(theta * (1.0f - t)) * invsintheta;
		const double invscale = sin(theta * t) * invsintheta;
		return (*this = (q1 * scale) + (q2 * invscale));
	}
	else // linear interpolation
		return lerp(q1, q2, t);
}

Quaternion& Quaternion::identity()
{
	w = 1.0;
	x = 0.0;
	y = 0.0;
	z = 0.0;
	return *this;
}

Quaternion& Quaternion::inverse()
{
	x = -x; 
	y = -y; 
	z = -z;
	return *this;
}

Quaternion& Quaternion::normalize()
{
	return (*this *= (1.0 / sqrt(x * x + y * y + z * z + w * w)));
}

Quaternion Quaternion::getIdentity()
{
	return Quaternion(0.0, 0.0, 0.0, 0.1);
}

Quaternion Quaternion::getInverse()
{
	return Quaternion(-x, -y, -z, w);
}

Quaternion Quaternion::getNormalize()
{
	return Quaternion(x, y, z, w) *= (1.0 / sqrt(x * x + y * y + z * z + w * w));
}

Quaternion& Quaternion::rotationFromTo(const Vector3& from, const Vector3& to)
{
	// Based on Stan Melax's article in Game Programming Gems
	// Copy, since cannot modify local
	Vector3 v0 = from;
	Vector3 v1 = to;
	v0.normalize();
	v1.normalize();

	const double d = v0.dot(v1);
	if (d >= 1.0f) // If dot == 1, vectors are the same
	{
		return identity();
	}
	else if (d <= -1.0f) // exactly opposite
	{
		Vector3 axis(1.0f, 0.f, 0.f);
		axis = axis.cross(v0);
		if (axis.magnitude() == 0)
		{
			axis.set(0.f, 1.f, 0.f);
			axis = axis.cross(v0);
		}
		// same as fromAngleAxis(core::PI, axis).normalize();
		return set(axis.x, axis.y, axis.z, 0).normalize();
	}

	const double s = sqrt((1 + d) * 2); // optimize inv_sqrt
	const double invs = 1.f / s;
	const Vector3 c = v0.cross(v1) * invs;
	return set(c.x, c.y, c.z, s * 0.5f).normalize();
}

Vector3 Quaternion::toEuler()
{
	Vector3 euler;
	const double sqw = w * w;
	const double sqx = x * x;
	const double sqy = y * y;
	const double sqz = z * z;
	const double test = 2.0 * (y * w - x * z);

	const double a = test, b = 1.0, tolerance = 0.000001;
	if ((a + tolerance >= b) && (a - tolerance <= b))
	{
		// heading = rotation about z-axis
		euler.z = -2.0 * atan2(x, w);
		// bank = rotation about x-axis
		euler.x = 0;
		// attitude = rotation about y-axis
		euler.y = M_PI / 2.0;
	}
	else if ((a + tolerance >= -b) && (a - tolerance <= -b))
	{
		// heading = rotation about z-axis
		euler.z = 2.0 * atan2(x, w);
		// bank = rotation about x-axis
		euler.x = 0;
		// attitude = rotation about y-axis
		euler.y = M_PI / -2.0;
	}
	else
	{
		// heading = rotation about z-axis
		euler.z = atan2(2.0 * (x * y + z * w), (sqx - sqy - sqz + sqw));
		// bank = rotation about x-axis
		euler.x = atan2(2.0 * (y * z + x * w), (-sqx - sqy + sqz + sqw));
		// attitude = rotation about y-axis
		euler.y = asin(std::min(std::max(test, 1.0), 1.0));
	}
	return euler;
}

void Quaternion::toAngleAxis(double& angle, Vector3& axis)
{
	const double scale = sqrt(x * x + y * y + z * z);
	const double tolerance = 0.00000001;
	if (fabs(scale) <= tolerance || w > 1.0f || w < -1.0f)
	{
		angle = 0.0;
		axis.x = 0.0;
		axis.y = 1.0;
		axis.z = 0.0;
	}
	else
	{
		const double invscale = 1.0 / scale;
		angle = 2.0f * acos(w);
		axis.x = x * invscale;
		axis.y = y * invscale;
		axis.z = z * invscale;
	}
}

Quaternion Quaternion::AnglesToQuaternion(double yaw, double pitch, double roll)
{
	double _yaw = yaw * DEG_TO_RAD;
	double _pitch = pitch * DEG_TO_RAD;
	double _roll = roll * DEG_TO_RAD;

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

Quaternion Quaternion::hamilton(const Quaternion& q1, const Quaternion& q2)
{
	Quaternion aux;
	aux.w = q1.w * q2.w - q1.x * q2.x - q1.y * q2.y - q1.z * q2.z;
	aux.x = q1.w * q2.x + q1.x * q2.w + q1.y * q2.z - q1.z * q2.y;
	aux.y = q1.w * q2.y - q1.x * q2.z + q1.y * q2.w + q1.z * q2.x;
	aux.z = q1.w * q2.z + q1.x * q2.y - q1.y * q2.x + q1.z * q2.w;
	return aux;
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
