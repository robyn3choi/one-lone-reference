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
		if (mDashTimer <= 0)
		{
			mIsDashing = false;
			mDashTimer = DASH_TIME;
		}
	}

	// TODO: refactor?
	Texture* tex = GameManager::Instance().GetTextureManager()->GetTexture(TextureType::Player);
	float texWidth = tex->GetWidth();
	float texHeight = tex->GetHeight();

	float nextX = mPosition.x + mVelocity.x * deltaTime;
	float nextY = mPosition.y + mVelocity.y * deltaTime;

	if (nextX <= LEVEL_WIDTH - texWidth && nextX >= 0)
	{
		mPosition.x = nextX;
		mCollider.x = mPosition.x;
	}
	if (nextY <= LEVEL_HEIGHT - texHeight && nextY >= 0)
	{
		mPosition.y = nextY;
		mCollider.y = mPosition.y;
	}
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
