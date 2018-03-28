#pragma once
#include "GameObject.h"
#include "Texture.h"

class Bullet : public GameObject
{
public:
	Bullet(Texture* texture, SDL_Rect& camera, SDL_Rect& collider);
	~Bullet();

	void Update(Uint32 deltaTime) override;
	void Shoot(Vector2 startPos, Vector2 velocity);

private:
	Vector2 mVelocity;
};

