#pragma once
#include "GameObject.h"
class Enemy :
	public GameObject
{
public:
	Enemy(TextureType textureType);
	~Enemy();

	void Update(float deltaTime) override;
	void TakeDamage();

private:
	int mHealth = ENEMY_HEALTH;
	float mSpeed = ENEMY_SPEED;
};

