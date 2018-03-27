#pragma once
class Vector2
{
public:
	Vector2();
	Vector2(float x, float y);
	~Vector2();

	float x;
	float y;

	float GetLength() const;
	void Normalize();
	void operator+=(const Vector2& rhs);
	void operator-=(const Vector2& rhs);
};

Vector2 operator+(const Vector2& lhs, const Vector2& rhs);
Vector2 operator-(const Vector2& lhs, const Vector2& rhs);
Vector2 operator*(float lhs, const Vector2& rhs);
Vector2 operator*(const Vector2& lhs, float rhs);
Vector2 operator/(const Vector2& lhs, float rhs);


