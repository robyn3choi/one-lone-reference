#include "Precompiled.h"
#include "EnemySpawner.h"


EnemySpawner::EnemySpawner(std::vector<Enemy*>& enemies) :
	mEnemies(enemies)
{
}


EnemySpawner::~EnemySpawner()
{
}

void EnemySpawner::SpawnInitialEnemies()
{
	SpawnEnemy();

	for (int i = 1; i < NUM_INITIAL_ENEMIES;i++)
	{
		SDL_TimerID timerID = SDL_AddTimer(500 * i, SpawnEnemyCallback, this);
	}
}

Uint32 EnemySpawner::SpawnEnemyCallback(Uint32 interval, void* param)
{
	((EnemySpawner*)param)->SpawnEnemy();
	return 0;
}

bool EnemySpawner::SpawnEnemy()
{
	if (mEnemyIndex == mEnemies.size())
	{
		return false;
	}
	mEnemies[mEnemyIndex]->Spawn();
	mEnemyIndex++;
	return true;
}

void EnemySpawner::Reset()
{
	for (Enemy* enemy : mEnemies)
	{
		enemy->Reset();
	}
}
