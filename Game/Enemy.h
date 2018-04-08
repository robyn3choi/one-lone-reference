#pragma once
#include "GameObject.h"
class Enemy :
	public GameObject
{
public:
	Enemy(TextureType textureType);
	~Enemy();

	void Update(Uint32 deltaTime) override;
};

