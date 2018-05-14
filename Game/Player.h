#pragma once
#include "GameObject.h"
#include "Texture.h"

class Player : public GameObject
{
public:
	Player(TextureType textureType, BulletPool* bulletPool);
	~Player();

	virtual void Update(float deltaTime) override;
	virtual void Reset() override;
	virtual TextureType GetTextureType() const override;
	void SetMoveDirection(Vector2 dir);
	void TakeDamage();
	void Shoot(Vector2 dir);
	void Dash(Vector2 dir);
	int GetHealth() { return mHealth; }
	bool IsDashing() { return mIsDashing; }

private:
	float mSpeed = PLAYER_SPEED;
	int mHealth = PLAYER_HEALTH;
	bool mIsFiring = false;
	float mFireTimer = PLAYER_FIRE_RATE;
	bool mIsDashing = false;
	float mDashTimer = DASH_TIME;
	BulletPool* mBulletPool = nullptr;
};

