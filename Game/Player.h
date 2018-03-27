#pragma once
#include "GameObject.h"
#include "Texture.h"

class Player : public GameObject
{
public:
	Player(Texture* texture, float speed, SDL_Rect& mCamera);
	~Player();

	void Update(Uint32 deltaTime) override;
	void Move(Vector2 dir);

private:
	float mSpeed;
	SDL_Rect& mCamera;
};

