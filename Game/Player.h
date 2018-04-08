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

private:
	float mSpeed = PLAYER_SPEED;
	float mHealth = PLAYER_HEALTH;
	bool mIsDashing = false;
	float mDashTimer = DASH_TIME;
};

