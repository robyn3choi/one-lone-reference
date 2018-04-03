#pragma once
#include "GameObject.h"
#include "Texture.h"

class Bullet : public GameObject
{
public:
	Bullet(Texture& texture, SDL_Rect& collider, float speed, bool hurtsPlayer);
	~Bullet();

	void Update(Uint32 deltaTime) override;
	void Shoot(Vector2 startPos, Vector2 velocity);

private:
	Vector2 mVelocity;
	float mSpeed;
	bool mHurtsPlayer;
};

