#include "GameObject.h"

GameObject::GameObject(Texture& texture, SDL_Rect& collider)
{
}


GameObject::~GameObject()
{
	mTexture = nullptr;
}

void GameObject::Update(Uint32 deltaTime)
{
	if (!mIsActive)
	{
		return;
	}

	mPosition += mVelocity * deltaTime;
	mCollider.x = mPosition.x;
	mCollider.y = mPosition.y;
}

void GameObject::Render(SDL_Rect & camera)
{
	mTexture->Render(mPosition.x - camera.x, mPosition.y - camera.y);
}

void GameObject::SetActive(bool active)
{
	mIsActive = active;
}

bool GameObject::IsActive()
{
	return mIsActive;
}

const Vector2& GameObject::GetVelocity() const
{
	return mVelocity;
}

const Vector2& GameObject::GetPosition() const
{
	return mPosition;
}

void GameObject::SetPosition(Vector2& position)
{
	mPosition = position;
	mCollider.x = mPosition.x;
	mCollider.y = mPosition.y;
}

void GameObject::SetVelocity(Vector2& velocity)
{
	mVelocity = velocity;
}

int GameObject::GetWidth() const
{
	return mTexture->GetWidth();
}

int GameObject::GetHeight() const
{
	return mTexture->GetHeight();
}
