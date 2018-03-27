#include "Bullet.h"

Bullet::Bullet(Texture* texture)
	: GameObject(texture)
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
	mTexture->Render(mPosition.X(), mPosition.Y());
}
