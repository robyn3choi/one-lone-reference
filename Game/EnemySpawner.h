#pragma once
#include "Enemy.h"

class EnemySpawner
{
public:
	EnemySpawner(std::vector<Enemy*>& enemies);
	~EnemySpawner();

	void SpawnInitialEnemies();
	// must be static to be a callback for SDL_Timer
	static Uint32 SpawnEnemyCallback(Uint32 interval, void* param);
	bool SpawnEnemy();
	

private:
	std::vector<Enemy*>& mEnemies;
	int mEnemyIndex = 0;
};

