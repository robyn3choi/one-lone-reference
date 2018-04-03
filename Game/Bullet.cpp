#include "Bullet.h"

Bullet::Bullet(Texture& texture, SDL_Rect& collider, bool hurtsPlayer) :
	mHurtsPlayer(hurtsPlayer),
	GameObject(texture, collider)
{
}


Bullet::~Bullet()
{
}

void Bullet::Initialize(Texture * texture, SDL_Rect & collider, float speed, bool hurtsPlayer)
{
	mTexture = texture;
	mCollider = collider;
	mSpeed = speed;
	mHurtsPlayer = mHurtsPlayer;
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
}
