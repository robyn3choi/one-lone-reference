#pragma once
#include "GameObject.h"
class Boss :
	public GameObject
{
public:
	Boss(TextureType textureType, Player* player, BulletPool* bulletPool);
	~Boss();

	virtual void Update(float deltaTime) override;
	virtual void Reset() override;
	int GetHealth() { return mHealth; }
	void TakeDamage();
	bool IsDead() { return mHealth <= 0; }
	virtual TextureType GetTextureType() const override;

private:
	void ShootAtPlayer(float deltaTime);
	void Spiral(float deltaTime);
	void MoveTowardPlayer(float deltaTime);
	void ShootFromBodyCenter(Vector2 direction);

	enum class AttackPattern {
		ShootAtPlayer,
		Spiral
	};
	int mHealth = BOSS_HEALTH;
	float mSpeed = BOSS_SPEED;
	float mFireTimer = BOSS_FIRE_RATE;
	float mAttackTimer = BOSS_ATTACK_DURATION;
	float mPauseTimer = BOSS_PAUSE_DURATION;
	AttackPattern mCurrentAttackPattern = AttackPattern::Spiral;
	BulletPool* mBulletPool = nullptr;
	std::vector<Vector2*> mShootDirections;
	Player* mPlayer = nullptr;
	bool mIsAttacking = false;
};

