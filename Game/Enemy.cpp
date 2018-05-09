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

	MoveTowardPlayer(deltaTime, vectorToPlayer);

	mFireTimer -= deltaTime;
	if (mFireTimer <= 0)
	{
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
	int randomX = rand() % LEVEL_WIDTH;
	int randomY = rand() % LEVEL_HEIGHT;
	mPosition.x = static_cast<float>(randomX);
	mPosition.y = static_cast<float>(randomY);
}

void Enemy::MoveTowardPlayer(float deltaTime, Vector2& vectorToPlayer)
{
	if (vectorToPlayer.GetLength() > ENEMY_DISTANCE_FROM_PLAYER)
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
}

void Enemy::Reset()
{
	mHealth = ENEMY_HEALTH;
	SetActive(false);
}
