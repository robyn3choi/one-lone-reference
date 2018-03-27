#include "Player.h"

Player::Player(Texture* texture, float speed, SDL_Rect& camera) :
	GameObject(texture),
	mSpeed(speed),
	mCamera(camera)
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
	mTexture->Render(mPosition.X() - mCamera.x, mPosition.Y() - mCamera.y);
}

void Player::Move(Vector2 dir)
{
	mVelocity = dir * mSpeed;
}
