#include "Vector2.h"

Vector2::Vector2()
{
}

Vector2::Vector2(float newX, float newY) :
	mX(newX),
	mY(newY)
{
}


Vector2::~Vector2()
{
}

void Vector2::operator+=(const Vector2 & rhs)
{
	mX += rhs.X();
	mY += rhs.Y();
}

void Vector2::operator-=(const Vector2 & rhs)
{
	mX -= rhs.X();
	mY -= rhs.Y();
}

Vector2 operator+(const Vector2 & lhs, const Vector2 & rhs)
{
	return Vector2(lhs.X() + rhs.X(), rhs.Y() + rhs.Y());
}

Vector2 operator-(const Vector2 & lhs, const Vector2 & rhs)
{
	return Vector2(lhs.X() - rhs.X(), rhs.Y() - rhs.Y());
}

Vector2 operator*(float lhs, const Vector2 & rhs)
{
	return Vector2(lhs * rhs.X(), lhs * rhs.Y());
}

Vector2 operator*(const Vector2 & lhs, float rhs)
{
	return Vector2(rhs * lhs.X(), rhs * lhs.Y());
}

Vector2 operator/(const Vector2 & lhs, float rhs)
{
	return Vector2(lhs.X() / rhs, lhs.Y() / rhs);
}
