#pragma once
#include "GameObject.h"
#include "Texture.h"

class Bullet : public GameObject
{
public:
	Bullet(TextureType textureType, const float speed, bool hurtsPlayer);
	~Bullet();

	void Update(float deltaTime) override;
	void Shoot(Vector2 startPos, Vector2 dir);

private:
	bool IsOutOfBounds();
	void Reset();

	Vector2 mVelocity;
	const float mSpeed;
	bool mHurtsPlayer;
	float mSecondsLeftUntilSelfDestruct = BULLET_LIFETIME;
};

