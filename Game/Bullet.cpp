#include "Bullet.h"

Bullet::Bullet(TextureType textureType, const float speed, bool hurtsPlayer) :
	GameObject(textureType),
	mSpeed(speed),
	mHurtsPlayer(hurtsPlayer)
{
}


Bullet::~Bullet()
{
}


void Bullet::Shoot(Vector2 startPos, Vector2 dir)
{
	dir.Normalize();
	mPosition = startPos;
	mCollider.x = static_cast<int>(mPosition.x);
	mCollider.y = static_cast<int>(mPosition.y);
	mVelocity = dir * mSpeed;
}

void Bullet::Update(float deltaTime)
{
	if (!mIsActive)
	{
		return;
	}

	mPosition += mVelocity * deltaTime;
	mCollider.x = static_cast<int>(mPosition.x);
	mCollider.y = static_cast<int>(mPosition.y);
}
