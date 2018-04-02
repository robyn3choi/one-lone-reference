#pragma once
#include <vector>
#include "GameObject.h"

template <typename T> 
class ObjectPool
{
	static_assert(std::is_base_of<GameObject, T>::value, "T must inherit from GameObject");

public:
	ObjectPool(const int size);
	~ObjectPool();

	T& GetObject();
	void ReturnObject(GameObject* object);

private:
	std::vector<T> mPool;
};

