#include "Bullet.h"

Bullet::Bullet(Texture* texture, SDL_Rect& camera, SDL_Rect& collider)
	: GameObject(texture, camera, collider)
{
}


Bullet::~Bullet()
{
}

void Bullet::Shoot(Vector2 startPos, Vector2 velocity)
{
	mPosition = startPos;
	mVelocity = velocity;
}

void Bullet::Update(Uint32 deltaTime)
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
