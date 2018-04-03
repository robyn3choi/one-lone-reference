#include "Player.h"

Player::Player(Texture& texture, SDL_Rect& collider, const float speed) :
	GameObject(texture collider),
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
	mTexture->Render(mPosition.x - mCamera.x, mPosition.y - mCamera.y);
}

void Player::Move(Vector2 dir)
{
	mVelocity = dir * mSpeed;
}
