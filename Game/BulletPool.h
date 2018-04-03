#pragma once
#include <vector>
#include "Bullet.h"

class BulletPool
{
public:
	BulletPool(const int size, Texture* texture, SDL_Rect& collider, const float speed, bool hurtsPlayer);
	~BulletPool();

	Bullet& GetBullet();
	void ReturnBullet(Bullet& bullet);

private:
	std::vector<Bullet> mPool;
};

