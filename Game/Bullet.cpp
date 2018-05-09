#include "Bullet.h"

Bullet::Bullet(TextureType textureType, const float speed, bool hurtsPlayer) :
	GameObject(textureType),
	mSpeed(speed),
	mHurtsPlayer(hurtsPlayer)
{
}


Bullet::~Bullet()
{
}

void Bullet::Update(float deltaTime)
{
	if (!mIsActive)
	{
		return;
	}

	mPosition += mVelocity * deltaTime;
	mCollider.x = static_cast<int>(mPosition.x);
	mCollider.y = static_cast<int>(mPosition.y);

	mSecondsLeftUntilSelfDestruct -= deltaTime;

	if (mSecondsLeftUntilSelfDestruct <= 0 || IsOutOfBounds())
	{
		Reset();
	}
}

void Bullet::Shoot(Vector2 startPos, Vector2 dir)
{
	dir.Normalize();
	mPosition = startPos;
	mCollider.x = static_cast<int>(mPosition.x);
	mCollider.y = static_cast<int>(mPosition.y);
	mVelocity = dir * mSpeed;
}

bool Bullet::IsOutOfBounds()
{
	int textureWidth = GameManager::Instance().GetTextureManager()->GetTexture(TextureType::PlayerBullet)->GetWidth();
	int textureHeight = GameManager::Instance().GetTextureManager()->GetTexture(TextureType::PlayerBullet)->GetHeight();

	if (static_cast<int>(mPosition.x) > LEVEL_WIDTH - textureWidth ||
		static_cast<int>(mPosition.x) < 0 ||
		static_cast<int>(mPosition.y) > LEVEL_HEIGHT - textureHeight ||
		static_cast<int>(mPosition.y) < 0)
	{
		return true;
	}
	return false;
}

void Bullet::Reset()
{
	mSecondsLeftUntilSelfDestruct = BULLET_LIFETIME;
	SetActive(false);
}
