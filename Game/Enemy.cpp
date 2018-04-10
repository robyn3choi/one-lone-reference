#include "Precompiled.h"
#include "Enemy.h"
#include <time.h> 

Enemy::Enemy(TextureType textureType, Player* player, BulletPool* bulletPool) :
	GameObject(textureType),
	mPlayer(player),
	mBulletPool(bulletPool)
{
	//srand(time(NULL));
	//int randomFactor = rand() % 5;
	//mDistanceFromPlayer = ENEMY_DISTANCE_FROM_PLAYER + (float)(randomFactor * 100);
	//std::cout << randomFactor << std::endl;
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

	Vector2 vectorToPlayer = mPlayer->GetPosition() - mPosition;

	if (vectorToPlayer.GetLength() > mDistanceFromPlayer)
	{
		vectorToPlayer.Normalize();
		mVelocity = vectorToPlayer * mSpeed;
	}
	else
	{
		mVelocity = Vector2(0, 0);
	}

	mPosition += mVelocity * deltaTime;
	mCollider.x = mPosition.x;
	mCollider.y = mPosition.y;

	mFireTimer -= deltaTime;
	if (mFireTimer <= 0)
	{
		Bullet* bullet = mBulletPool->GetBullet();
		bullet->Shoot(mPosition, vectorToPlayer);
		mFireTimer = ENEMY_FIRE_RATE;
	}
}

void Enemy::TakeDamage()
{
	if (--mHealth <= 0)
	{
		// TODO: DIE
		std::cout << "enemy ded" << std::endl;
	}
}