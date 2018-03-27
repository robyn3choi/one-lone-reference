#include "Vector2.h"

Vector2::Vector2()
{
}

Vector2::Vector2(float newX, float newY) :
	x(newX),
	y(newY)
{
}


Vector2::~Vector2()
{
}

float Vector2::GetLength() const
{
	return sqrt(x * x + y * y);
}

void Vector2::Normalize()
{
	float length = GetLength();
	if (length == 0)
	{
		printf("Trying to normalize a zero vector");
	}
	else
	{
		x /= length;
		y /= length;
	}
}

void Vector2::operator+=(const Vector2 & rhs)
{
	x += rhs.x;
	y += rhs.y;
}

void Vector2::operator-=(const Vector2 & rhs)
{
	x -= rhs.x;
	y -= rhs.y;
}

Vector2 operator+(const Vector2 & lhs, const Vector2 & rhs)
{
	return Vector2(lhs.x + rhs.x, rhs.y + rhs.y);
}

Vector2 operator-(const Vector2 & lhs, const Vector2 & rhs)
{
	return Vector2(lhs.x - rhs.x, lhs.y - rhs.y);
}

Vector2 operator*(float lhs, const Vector2 & rhs)
{
	return Vector2(lhs * rhs.x, lhs * rhs.y);
}

Vector2 operator*(const Vector2 & lhs, float rhs)
{
	return Vector2(rhs * lhs.x, rhs * lhs.y);
}

Vector2 operator/(const Vector2 & lhs, float rhs)
{
	return Vector2(lhs.x / rhs, lhs.y / rhs);
}
