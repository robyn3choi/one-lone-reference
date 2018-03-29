#pragma once
#include <vector>
#include "GameObject.h"

class GameObjectPool
{
public:
	GameObjectPool(const int size, GameObject* object);
	~GameObjectPool();

	GameObject*& GetGameObject();
	void ReturnGameObject(GameObject* object);

private:
	std::vector<GameObject*> mPool;
};

