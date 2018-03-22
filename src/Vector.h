#pragma once

#include "stdio.h"
#include "math.h"

class Vector2f
{
public:
	Vector2f()
	{
		x = y = 0.0f;
	}
	Vector2f(float x, float y)
	{
		this->x = x;
		this->y = y;
	}
	float x, y;
	float GetLength()
	{
		return sqrtf(this->x*this->x + this->y*this->y);
	}
};

Vector2f operator+(Vector2f left, Vector2f right)
{
	return Vector2f(left.x + right.x, left.y + right.y);
}

Vector2f operator-(Vector2f left, Vector2f right)
{
	return Vector2f(left.x - right.x, left.y - right.y);
}

Vector2f operator*(Vector2f v, float value)
{
	return Vector2f(v.x * value, v.y * value);
}

Vector2f operator*(float value, Vector2f v)
{
	return Vector2f(v.x * value, v.y * value);
}

float operator*(Vector2f v1, Vector2f v2)
{
	return v1.x*v2.x + v1.y*v2.y;
}

float operator^(Vector2f v1, Vector2f v2)
{
	return v1.x*v2.y - v1.y*v2.x;
}
