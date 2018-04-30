#pragma once
#include "Enemy.h"

class EnemySpawner
{
public:
	EnemySpawner(std::vector<std::unique_ptr<Enemy>>& enemies);
	~EnemySpawner();

	void SpawnInitialEnemies();
	// must be static to be a callback for SDL_Timer
	static Uint32 SpawnEnemyCallback(Uint32 interval, void* param);
	bool SpawnEnemy();
	void Reset();

private:
	std::vector<std::unique_ptr<Enemy>>& mEnemies;
	int mEnemyIndex = 0;
};

