#include "GameObject.h"

GameObject::GameObject(TextureType textureType) :
	mTextureType(textureType)
{
	//TODO: CHANGE SO THAT THE COLLIDER IS SMALLER THAN THE TEXTURE
	int width = GameManager::Instance().GetTextureManager()->GetTexture(mTextureType)->GetWidth();
	int height = GameManager::Instance().GetTextureManager()->GetTexture(mTextureType)->GetHeight();
	mCollider = { 0, 0, width, height };
}


GameObject::~GameObject()
{
}

void GameObject::SetPosition(Vector2 position)
{
	mPosition = std::move(position);
	mCollider.x = static_cast<int>(mPosition.x);
	mCollider.y = static_cast<int>(mPosition.y);
}