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
	mCollider.x = mPosition.x;
	mCollider.y = mPosition.y;
	mVelocity = dir * mSpeed;
}

void Bullet::Update(float deltaTime)
{
	if (!mIsActive)
	{
		return;
	}

	mPosition += mVelocity * deltaTime;
	mCollider.x = mPosition.x;
	mCollider.y = mPosition.y;
}
