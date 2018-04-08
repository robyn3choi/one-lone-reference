#include "GameObject.h"

GameObject::GameObject(TextureType textureType) :
	mTextureType(textureType)
{
	//TODO: CHANGE SO THAT THE COLLIDER IS SMALLER THAN THE TEXTURE
	int width = TextureManager::GetTexture(mTextureType)->GetWidth();
	int height = TextureManager::GetTexture(mTextureType)->GetHeight();
	mCollider = { 0, 0, width, height };
}


GameObject::~GameObject()
{
}

void GameObject::Render(SDL_Rect & camera)
{
	TextureManager::GetTexture(mTextureType)->Render(mPosition.x - camera.x, mPosition.y - camera.y);
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

const SDL_Rect GameObject::GetCollider() const
{
	return mCollider;
}
