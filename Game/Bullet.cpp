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


void Bullet::Shoot(Vector2 startPos, Vector2 velocity)
{
	mPosition = startPos;
	mCollider.x = mPosition.x;
	mCollider.y = mPosition.y;
	mVelocity = velocity;
}

void Bullet::Update(Uint32 deltaTime)
{
	if (!mIsActive)
	{
		return;
	}

	mPosition += mVelocity * deltaTime;
	mCollider.x = mPosition.x;
	mCollider.y = mPosition.y;
}
