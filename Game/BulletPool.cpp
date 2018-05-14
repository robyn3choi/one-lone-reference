#include "Precompiled.h"
#include "BulletPool.h"

BulletPool::BulletPool(const int size, TextureType textureType, const float speed, bool hurtsPlayer)
{
	mPool.reserve(size);
	for (int i = 0; i < size-1; i++)
	{
		auto bullet = std::make_unique<Bullet>(textureType, speed, hurtsPlayer);
		mPool.push_back(std::move(bullet));
		mPool[i]->SetActive(false);
	}
}

BulletPool::~BulletPool()
{
}

Bullet* BulletPool::GetBullet()
{
	for (size_t i = 0; i < mPool.size(); i++)
	{
		if (!mPool[i]->IsActive())
		{
			mPool[i]->SetActive(true);
			return mPool[i].get();
		}
	}
	printf("ran out of bullets!");
	return nullptr;
}

void BulletPool::ReturnBullet(Bullet* bullet)
{
	bullet->SetActive(false);
}

void BulletPool::Reset()
{
	for (size_t i = 0; i < mPool.size(); i++)
	{
		mPool[i]->SetActive(false);
	}
}



