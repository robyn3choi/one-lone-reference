#pragma once
#include <vector>
#include "Bullet.h"

class BulletPool
{
public:
	BulletPool(const int size, TextureType textureType, const float speed, bool hurtsPlayer);
	~BulletPool();

	Bullet* GetBullet();
	void ReturnBullet(Bullet* bullet);
	const std::vector<Bullet*>& GetPool() const;

private:
	std::vector<Bullet*> mPool;
};

