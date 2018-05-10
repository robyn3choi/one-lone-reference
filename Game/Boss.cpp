#include "Precompiled.h"
#include "Boss.h"


Boss::Boss(TextureType textureType, Player* player, BulletPool* bulletPool) :
	GameObject(textureType),
	mPlayer(player),
	mBulletPool(bulletPool)
{
	mShootDirections.reserve(7);

	float rotation = 45;
	for (int i = 0; i < 8; i++)
	{
		auto v = std::make_unique<Vector2>(0, 1);
		v->RotateCounterClockwise(rotation * i);
		mShootDirections.push_back(std::move(v));
	}
}


Boss::~Boss()
{
}

void Boss::Update(float deltaTime)
{
	if (mIsAttacking)
	{
		mAttackTimer -= deltaTime;
		if (mAttackTimer <= 0)
		{
			mIsAttacking = false;
			mAttackTimer = BOSS_ATTACK_DURATION;
			if (mCurrentAttackPattern == AttackPattern::ShootAtPlayer)
			{
				mCurrentAttackPattern = AttackPattern::Spiral;
			}
			else
			{
				mCurrentAttackPattern = AttackPattern::ShootAtPlayer;
			}
		}
		else
		{
			if (mCurrentAttackPattern == AttackPattern::ShootAtPlayer)
			{
				ShootAtPlayer(deltaTime);
			}
			else
			{
				Spiral(deltaTime);
			}
		}
	}
	else
	{
		MoveTowardPlayer(deltaTime);
		mPauseTimer -= deltaTime;
		if (mPauseTimer <= 0)
		{
			mIsAttacking = true;
			mPauseTimer = BOSS_PAUSE_DURATION;
		}
	}
}

void Boss::Reset()
{
	mHealth = BOSS_HEALTH;
	mFireTimer = BOSS_FIRE_RATE;
	mAttackTimer = BOSS_ATTACK_DURATION;
	mPauseTimer = BOSS_PAUSE_DURATION;
}

void Boss::TakeDamage()
{
	if (--mHealth <= 0)
	{
		SetActive(false);
		GameManager::Instance().Win();
	}
}

void Boss::ShootAtPlayer(float deltaTime)
{
	mFireTimer -= deltaTime;
	if (mFireTimer <= 0)
	{
		Vector2 vectorToPlayer = mPlayer->GetPosition() - mPosition;
		vectorToPlayer.Normalize();
		ShootFromBodyCenter(vectorToPlayer);
		mFireTimer = BOSS_FIRE_RATE;
	}
}

void Boss::Spiral(float deltaTime)
{
	for (auto& dir : mShootDirections)
	{
		dir->RotateCounterClockwise(BOSS_ROTATION_SPEED * deltaTime);
	}
	mFireTimer -= deltaTime;
	if (mFireTimer <= 0)
	{
		for (auto& dir : mShootDirections)
		{
			ShootFromBodyCenter(*dir);
		}
		mFireTimer = BOSS_FIRE_RATE;
	}
}

void Boss::MoveTowardPlayer(float deltaTime)
{
	Vector2 vectorToPlayer = mPlayer->GetPosition() - mPosition;

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

void Boss::ShootFromBodyCenter(Vector2 direction)
{
	float textureWidth = GameManager::Instance().GetTextureManager()->GetTexture(mTextureType)->GetWidth();
	float textureHeight = GameManager::Instance().GetTextureManager()->GetTexture(mTextureType)->GetHeight();
	Vector2 centreOfBody(mPosition.x + textureWidth / 2, mPosition.y + textureHeight / 2);
	mBulletPool->GetBullet()->Shoot(centreOfBody, direction);
}
