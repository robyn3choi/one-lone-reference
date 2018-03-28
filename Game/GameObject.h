#pragma once
#include "Texture.h"

class GameObject
{
public:
	GameObject(Texture* texture, SDL_Rect& mCamera, SDL_Rect& mCollider);
	virtual ~GameObject();

	virtual void Update(Uint32 deltaTime);
	void SetActive(bool active);
	const Vector2& GetPosition() const;
	const Vector2& GetVelocity() const;
	void SetPosition(Vector2& position);
	void SetVelocity(Vector2& velocity);
	int GetWidth() const;
	int GetHeight() const;

protected:
	Texture* mTexture = nullptr;
	SDL_Rect& mCamera;
	SDL_Rect& mCollider;
	Vector2 mPosition;
	Vector2 mVelocity;
	bool mIsActive = true;
};

