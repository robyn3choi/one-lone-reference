#include "Player.h"

Player::Player(TextureType textureType, BulletPool* bulletPool) :
	GameObject(textureType),
	mBulletPool(bulletPool)
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
		if (mDashTimer <= 0)
		{
			mIsDashing = false;
			mDashTimer = DASH_TIME;
		}
	}

	Texture* tex = GameManager::Instance().GetTextureManager()->GetTexture(TextureType::Player);
	float texWidth = static_cast<float>(tex->GetWidth());
	float texHeight = static_cast<float>(tex->GetHeight());

	float nextX = mPosition.x + mVelocity.x * deltaTime;
	float nextY = mPosition.y + mVelocity.y * deltaTime;

	if (nextX <= LEVEL_WIDTH - texWidth && nextX >= 0)
	{
		mPosition.x = nextX;
		mCollider.x = static_cast<int>(mPosition.x);
	}
	if (nextY <= LEVEL_HEIGHT - texHeight && nextY >= 0)
	{
		mPosition.y = nextY;
		mCollider.y = static_cast<int>(mPosition.y);
	}
}

void Player::SetMoveDirection(Vector2 dir)
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
		GameManager::Instance().HandlePlayerDeath();
	}
}

void Player::Shoot(Vector2 dir)
{
	mBulletPool->GetBullet()->Shoot(mPosition, dir);
}

void Player::Dash(Vector2 dir)
{
	mIsDashing = true;
	mVelocity = DASH_SPEED * dir;
}


void Player::Reset()
{
	mHealth = PLAYER_HEALTH;
}

TextureType Player::GetTextureType() const
{
	if (GameManager::Instance().HasWon())
	{
		return TextureType::CryingPlayer;
	}
	else
	{
		return mTextureType;
	}
}
