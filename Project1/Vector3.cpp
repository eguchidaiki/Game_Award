#include "Vector3.h"
#include <cmath>

Vector3::Vector3() 
{
}

Vector3::Vector3(float x, float y, float z) 
{
	this->x = x;
	this->y = y;
	this->z = z;
}

float Vector3::Length() const
{
	return sqrtf((x * x) + (y * y) + (z * z));
}

float Vector3::sqLength() const
{
	return (x * x) + (y * y) + (z * z);
}

Vector3& Vector3::Normalize()
{
	float len = Length();
	if (len != 0)
	{
		return *this /= len;
	}

	return *this;
}

float Vector3::Dot(const Vector3& v) const
{
	return (this->x * v.x) + (this->y * v.y) + (this->z * v.z);
}

Vector3 Vector3::Cross(const Vector3& v) const
{
	return Vector3(
		(this->y * v.z) - (this->z * v.y),
		(this->z * v.x) - (this->x * v.z),
		(this->x * v.y) - (this->y * v.x)
	);
}

Vector3 Vector3::operator+() const
{
	return *this;
}

Vector3 Vector3::operator-() const
{
	return Vector3(-this->x, -this->y, -this->z);
}

Vector3& Vector3::operator+=(const Vector3& v)
{
	this->x += v.x;
	this->y += v.y;
	this->z += v.z;
	return *this;
}

Vector3& Vector3::operator-=(const Vector3& v)
{
	this->x -= v.x;
	this->y -= v.y;
	this->z -= v.z;
	return *this;
}

Vector3& Vector3::operator*=(float s)
{
	x *= s;
	y *= s;
	z *= s;
	return *this;
}

Vector3& Vector3::operator/=(float s)
{
	x /= s;
	y /= s;
	z /= s;
	return *this;
}

bool Vector3::operator==(const Vector3& v)
{
	return this->x == v.x && this->y == v.y && this->z == v.z;
}

bool Vector3::operator!=(const Vector3& v)
{
	return !(*this == v);
}

const Vector3 operator+(const Vector3& v1, const Vector3& v2)
{
	Vector3 temp(v1);
	return temp += v2;
}

const Vector3 operator-(const Vector3& v1, const Vector3& v2)
{
	Vector3 temp(v1);
	return temp -= v2;
}

const Vector3 operator*(const Vector3& v, float s)
{
	Vector3 temp(v);
	return temp *= s;
}

const Vector3 operator*(float s, const Vector3& v)
{
	return v * s;
}

const Vector3 operator/(const Vector3& v, float s)
{
	Vector3 temp(v);
	return temp /= s;
}

// ê¸å`ï‚ä‘
const Vector3 Lerp(const Vector3& start, const Vector3& end, const float t)
{
	// float y = t;
	// return start * (1.0f - y) + end * y;
	return start * (1.0f - t) + end * t;
}

// easeInï‚ä‘
//const Vector3 easeIn(const Vector3& start, const Vector3& end, const float t)
//{
//	float y = t * t;
//	return start * (1.0f - y) + end * y;
//}

// easeOutï‚ä‘
//const Vector3 easeOut(const Vector3& start, const Vector3& end, const float t)
//{
//	float y = t * (2 - t);
//	return start * (1.0f - y) + end * y;
//}

// easeInOutï‚ä‘
//const Vector3 easeInOut(const Vector3& start, const Vector3& end, const float t)
//{
//	float y = t * t * (3 - 2 * t);
//	return start * (1.0f - y) + end * y;
//}
