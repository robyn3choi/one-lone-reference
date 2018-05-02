#pragma once
class Vector2
{
public:
	Vector2();
	Vector2(float x, float y);
	Vector2(const Vector2& other);
	Vector2(Vector2&& other);
	Vector2& operator=(const Vector2& rhs);
	Vector2& operator=(Vector2&& rhs);
	~Vector2();

	float x = 0;
	float y = 0;

	float GetLength() const;
	void Normalize();
	bool operator==(const Vector2& rhs) const;
	bool operator!=(const Vector2& rhs) const;
	void operator+=(const Vector2& rhs);
	void operator-=(const Vector2& rhs);
};

Vector2 operator+(const Vector2& lhs, const Vector2& rhs);
Vector2 operator-(const Vector2& lhs, const Vector2& rhs);
Vector2 operator*(float lhs, const Vector2& rhs);
Vector2 operator*(const Vector2& lhs, float rhs);
Vector2 operator/(const Vector2& lhs, float rhs);


