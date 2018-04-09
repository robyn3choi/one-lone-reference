#include "Player.h"

Player::Player(TextureType textureType) :
	GameObject(textureType)
{
}

Player::~Player()
{
}

void Player::Update(float deltaTime)
{
	if (!mIsActive)
	{
		return;
	}

	if (mIsDashing)
	{
		mDashTimer -= deltaTime;
		std::cout << mDashTimer << std::endl;
		if (mDashTimer <= 0)
		{
			mIsDashing = false;
			mDashTimer = DASH_TIME;
		}
	}

	mPosition += mVelocity * deltaTime;
	mCollider.x = mPosition.x;
	mCollider.y = mPosition.y;
}

void Player::Move(Vector2 dir)
{
	if (!mIsDashing)
	{
		mVelocity = dir * mSpeed;
	}
}

void Player::TakeDamage()
{
	if (--mHealth <= 0)
	{
		// TODO: DIE
		std::cout << "ded" << std::endl;
	}
}

void Player::Dash(Vector2 dir)
{
	mIsDashing = true;
	mVelocity = DASH_SPEED * dir;
}

bool Player::IsDashing()
{
	return mIsDashing;
}
