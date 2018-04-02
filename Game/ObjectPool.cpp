#include "Precompiled.h"
#include "ObjectPool.h"

template<typename T>
ObjectPool<T>::ObjectPool(const int size)
{
	mPool.reserve(size);
	for (int i = 0; i < size-1; i++)
	{
		T newObject = new T;
		mPool.push_back(newObject);
		newObject.SetActive(false);
	}
}

template<typename T>
ObjectPool::~ObjectPool()
{
	mPool.clear();
}

template<typename T>
T& ObjectPool::GetObject()
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

template<typename T>
void ObjectPool::ReturnObject(GameObject* object)
{
	object->SetActive(false);
}
