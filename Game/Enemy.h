#pragma once
#include "GameObject.h"
#include "Player.h"

class Enemy :
	public GameObject
{
public:
	Enemy(TextureType textureType, Player* player);
	~Enemy();

	void Update(float deltaTime) override;
	void TakeDamage();

private:
	int mHealth = ENEMY_HEALTH;
	float mSpeed = ENEMY_SPEED;
	Player* mPlayer = nullptr;
};

