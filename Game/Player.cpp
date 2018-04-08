#include "Player.h"

Player::Player(TextureType textureType, const float speed) :
	GameObject(textureType),
	mSpeed(speed)
{
}


Player::~Player()
{
}

void Player::Update(Uint32 deltaTime)
{
	if (!mIsActive)
	{
		return;
	}

	mPosition += mVelocity * deltaTime;
	mCollider.x = mPosition.x;
	mCollider.y = mPosition.y;
}

void Player::Move(Vector2 dir)
{
	mVelocity = dir * mSpeed;
}
