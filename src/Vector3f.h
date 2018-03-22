#pragma once

#include "stdio.h"
#include "math.h"

class Vector3f
{
public:
	Vector3f()
	{
		x = y = z = 0.0f;
	}
	Vector3f(float x, float y, float z)
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}
	float x, y, z;
	float GetLength()
	{
		return sqrtf(this->x*this->x + this->y*this->y + this->z*this->z);
	}
};



Vector3f operator+(Vector3f left, Vector3f right)
{
	return Vector3f(left.x + right.x, left.y + right.y, left.z + right.z);
}

Vector3f operator-(Vector3f left, Vector3f right)
{
	return Vector3f(left.x - right.x, left.y - right.y, left.z - right.z);
}

Vector3f & operator+=(Vector3f &v1, Vector3f &v2)
{
	v1.x += v2.x;
	v1.y += v2.y;
	v1.z += v2.z;
	return v1;
}

Vector3f & operator-=(Vector3f &v1, Vector3f &v2)
{
	v1.x -= v2.x;
	v1.y -= v2.y;
	v1.z -= v2.z;
	return v1;
}

Vector3f operator*(Vector3f v, float value)
{
	return Vector3f(v.x * value, v.y * value, v.z * value);
}

Vector3f operator*(float value, Vector3f v)
{
	return Vector3f(v.x * value, v.y * value, v.z * value);
}

float operator*(Vector3f v1, Vector3f v2)
{
	return v1.x*v2.x + v1.y*v2.y + v1.z*v2.z;
}
