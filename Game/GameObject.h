#pragma once
#include "Texture.h"

class GameObject
{
public:
	GameObject(TextureType textureType);
	virtual ~GameObject();

	virtual void Update(float deltaTime) = 0;
	virtual void Reset() = 0;
	void SetActive(bool active) { mIsActive = active; }
	bool IsActive() const { return mIsActive; }
	const Vector2& GetPosition() const { return mPosition; }
	const Vector2& GetVelocity() const { return mVelocity; }
	void SetPosition(Vector2 position);
	void SetVelocity(Vector2& velocity) { mVelocity = velocity; }
	const SDL_Rect GetCollider() const { return mCollider; }
	TextureType GetTextureType() const { return mTextureType; }
	
protected:
	SDL_Rect mCollider;
	Vector2 mPosition;
	Vector2 mVelocity;
	bool mIsActive = true;
	TextureType mTextureType;
};

