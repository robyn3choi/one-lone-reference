#include "Vector2.h"

Vector2::Vector2()
{
}

Vector2::Vector2(float newX, float newY) :
	x(newX), y(newY)
{
}

Vector2::Vector2(const Vector2 & other) :
	x(other.x), y(other.y)
{
}

Vector2::Vector2(Vector2 && other) :
	x(other.x), y(other.y)
{
	other.x = 0;
	other.y = 0;
}

Vector2 & Vector2::operator=(const Vector2 & rhs)
{
	x = rhs.x;
	y = rhs.y;
	return *this;
}

Vector2 & Vector2::operator=(Vector2 && rhs)
{
	if (*this != rhs)
	{
		x = rhs.x;
		y = rhs.y;
		rhs.x = 0;
		rhs.y = 0;
	}
	return *this;
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

void Vector2::RotateCounterClockwise(float degrees)
{
	float radians = degrees * static_cast<float>(M_PI) / 180;
	float cos = std::cos(radians);
	float sin = std::sin(radians);

	float oldX = x;
	float oldY = y;

	x = oldX * cos - y * sin;
	y = oldX * sin + oldY * cos;
}

bool Vector2::operator==(const Vector2 & rhs) const
{
	return rhs.x == x && rhs.y == y;
}

bool Vector2::operator!=(const Vector2 & rhs) const
{
	return rhs.x != x || rhs.y != y;
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
	return Vector2(lhs.x + rhs.x, lhs.y + rhs.y);
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
