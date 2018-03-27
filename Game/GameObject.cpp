#include "GameObject.h"

GameObject::GameObject(Texture* texture) :
	mTexture(texture)
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
	mTexture->Render(mPosition.x, mPosition.y);
}

void GameObject::SetActive(bool active)
{
	mIsActive = active;
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
