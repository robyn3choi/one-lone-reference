#include "Precompiled.h"
#include "GameObjectPool.h"


GameObjectPool::GameObjectPool(const int size, GameObject* object)
{
	mPool.reserve(size);
	mPool.push_back(object);
	for (int i = 0; i < size-1; i++)
	{
		GameObject* newObject = new GameObject(*object);
		mPool.push_back(newObject);
		newObject->SetActive(false);
	}
}


GameObjectPool::~GameObjectPool()
{
	mPool.clear();
}

GameObject*& GameObjectPool::GetGameObject()
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

void GameObjectPool::ReturnGameObject(GameObject* object)
{
	object->SetActive(false);
}
