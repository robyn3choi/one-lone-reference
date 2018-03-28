#pragma once
#include "GameObject.h"
#include "Texture.h"

class Player : public GameObject
{
public:
	Player(Texture* texture, SDL_Rect& camera, SDL_Rect& collider, float speed);
	~Player();

	void Update(Uint32 deltaTime) override;
	void Move(Vector2 dir);

private:
	float mSpeed;
};

