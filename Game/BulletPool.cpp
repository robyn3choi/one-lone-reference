#include "Precompiled.h"
#include "BulletPool.h"

BulletPool::BulletPool(const int size, TextureType textureType, const float speed, bool hurtsPlayer)
{
	mPool.reserve(size);
	for (int i = 0; i < size-1; i++)
	{
		Bullet* bullet = new Bullet(textureType, speed, hurtsPlayer);
		mPool.push_back(bullet);
		bullet->SetActive(false);
	}
}

BulletPool::~BulletPool()
{
}

Bullet* BulletPool::GetBullet()
{
	for (int i = 0; i < mPool.size(); i++)
	{
		if (!mPool[i]->IsActive())
		{
			mPool[i]->SetActive(true);
			return mPool[i];
		}
	}
}

void BulletPool::ReturnBullet(Bullet* bullet)
{
	bullet->SetActive(false);
}

const std::vector<Bullet*>& BulletPool::GetPool() const
{
	return mPool;
}

