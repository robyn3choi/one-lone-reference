#pragma once
#include "Texture.h"

class GameObject
{
public:
	GameObject(TextureType textureType);
	virtual ~GameObject();

	virtual void Update(float deltaTime) = 0;
	void Render(SDL_Rect& camera);
	void SetActive(bool active);
	bool IsActive();
	const Vector2& GetPosition() const;
	const Vector2& GetVelocity() const;
	void SetPosition(Vector2& position);
	void SetVelocity(Vector2& velocity);
	const SDL_Rect GetCollider() const;

protected:
	SDL_Rect mCollider;
	Vector2 mPosition;
	Vector2 mVelocity;
	bool mIsActive = true;
	TextureType mTextureType;
};

