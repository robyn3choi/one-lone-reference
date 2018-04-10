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

	void Update(float deltaTime) override;
	void TakeDamage();

private:
	int mHealth = ENEMY_HEALTH;
	float mSpeed = ENEMY_SPEED;
	Player* mPlayer = nullptr;
	BulletPool* mBulletPool = nullptr;
	bool mIsFiring = false;
	float mFireTimer = ENEMY_FIRE_RATE;
	float mDistanceFromPlayer = ENEMY_DISTANCE_FROM_PLAYER;
};

