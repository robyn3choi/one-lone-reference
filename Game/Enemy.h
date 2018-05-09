#pragma once
#include "GameObject.h"
#include "Player.h"
#include "BulletPool.h"

class Enemy :
	public GameObject
{
public:
	Enemy(TextureType textureType, Player* player, BulletPool* bulletPool);
	~Enemy();

	virtual void Update(float deltaTime) override;
	virtual void Reset() override;
	void TakeDamage();
	void Spawn();

private:
	void MoveTowardPlayer(float deltaTime, Vector2& vectorToPlayer);

	int mHealth = ENEMY_HEALTH;
	float mSpeed = ENEMY_SPEED;
	Player* mPlayer = nullptr;
	BulletPool* mBulletPool = nullptr;
	bool mIsFiring = false;
	float mFireTimer = ENEMY_FIRE_RATE;
};

