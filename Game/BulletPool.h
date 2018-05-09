#pragma once
#include <vector>
#include "Bullet.h"

class BulletPool
{
public:
	BulletPool(const int size, TextureType textureType, const float speed, bool hurtsPlayer);
	~BulletPool();

	Bullet* GetBullet();
	const std::vector<std::unique_ptr<Bullet>>& GetPool() const { return mPool; }

private:
	std::vector<std::unique_ptr<Bullet>> mPool;
};

