#pragma once
class Vector2
{
public:
	Vector2();
	Vector2(float x, float y);
	~Vector2();

	float X() const { return mX; }
	float Y() const { return mY; }
	void operator+=(const Vector2& rhs);
	void operator-=(const Vector2& rhs);

private:
	float mX = 0;
	float mY = 0;
};

Vector2 operator+(const Vector2& lhs, const Vector2& rhs);
Vector2 operator-(const Vector2& lhs, const Vector2& rhs);
Vector2 operator*(float lhs, const Vector2& rhs);
Vector2 operator*(const Vector2& lhs, float rhs);
Vector2 operator/(const Vector2& lhs, float rhs);


