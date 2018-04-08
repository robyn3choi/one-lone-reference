#pragma once
#include "GameObject.h"
#include "Texture.h"

class Player : public GameObject
{
public:
	Player(TextureType textureType, const float speed);
	~Player();

	void Update(Uint32 deltaTime) override;
	void Move(Vector2 dir);

private:
	float mSpeed;
};

