#include "Precompiled.h"
#include "Enemy.h"


Enemy::Enemy(TextureType textureType) :
	GameObject(textureType)
{
}


Enemy::~Enemy()
{
}

void Enemy::Update(float deltaTime)
{
	if (!mIsActive)
	{
		return;
	}

	mPosition += mVelocity * deltaTime;
	mCollider.x = mPosition.x;
	mCollider.y = mPosition.y;
}

void Enemy::TakeDamage()
{
	if (--mHealth <= 0)
	{
		// TODO: DIE
		std::cout << "enemy ded" << std::endl;
	}
}
