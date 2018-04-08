#include "Precompiled.h"
#include "Enemy.h"


Enemy::Enemy(TextureType textureType) :
	GameObject(textureType)
{
}


Enemy::~Enemy()
{
}

void Enemy::Update(Uint32 deltaTime)
{
	if (!mIsActive)
	{
		return;
	}

	mPosition += mVelocity * deltaTime;
	mCollider.x = mPosition.x;
	mCollider.y = mPosition.y;
}
