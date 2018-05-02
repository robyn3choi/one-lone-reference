#pragma once
#include "GameObject.h"
#include "Texture.h"

class Player : public GameObject
{
public:
	Player(TextureType textureType);
	~Player();

	void Update(float deltaTime) override;
	void Move(Vector2 dir);
	void TakeDamage();
	void Dash(Vector2 dir);
	bool IsDashing();
	void Reset();

private:
	float mSpeed = PLAYER_SPEED;
	int mHealth = PLAYER_HEALTH;
	bool mIsFiring = false;
	float mFireTimer = PLAYER_FIRE_RATE;
	bool mIsDashing = false;
	float mDashTimer = DASH_TIME;
};

