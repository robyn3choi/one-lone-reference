#include "Precompiled.h"
#include "Enemy.h"
#include <time.h> 

Enemy::Enemy(TextureType textureType, Player* player, BulletPool* bulletPool) :
	GameObject(textureType),
	mPlayer(player),
	mBulletPool(bulletPool)
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
	mCollider.x = static_cast<int>(mPosition.x);
	mCollider.y = static_cast<int>(mPosition.y);

	mFireTimer -= deltaTime;
	if (mFireTimer <= 0)
	{
		//Bullet* bullet = mBulletPool->GetBullet();
		mBulletPool->GetBullet()->Shoot(mPosition, vectorToPlayer);
		mFireTimer = ENEMY_FIRE_RATE;
	}
}

void Enemy::TakeDamage()
{
	if (--mHealth <= 0)
	{
		SetActive(false);
		GameManager::Instance().HandleEnemyDeath();
	}
}

void Enemy::Spawn()
{
	mIsActive = true;
	int randomFactor = rand() % 5;
	mDistanceFromPlayer = ENEMY_DISTANCE_FROM_PLAYER + (float)(randomFactor * 100);

	int randomX = rand() % LEVEL_WIDTH;
	int randomY = rand() % LEVEL_HEIGHT;
	mPosition.x = static_cast<float>(randomX);
	mPosition.y = static_cast<float>(randomY);
}

void Enemy::Reset()
{
	mHealth = ENEMY_HEALTH;
	SetActive(false);
}
