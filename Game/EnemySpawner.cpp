#include "Precompiled.h"
#include "EnemySpawner.h"


EnemySpawner::EnemySpawner(std::vector<std::unique_ptr<Enemy>>& enemies) :
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
		SDL_AddTimer(500 * i, SpawnEnemyCallback, this);
	}
}

Uint32 EnemySpawner::SpawnEnemyCallback(Uint32 interval, void* param)
{
	((EnemySpawner*)param)->SpawnEnemy();
	return 0;
}

void EnemySpawner::SpawnEnemy()
{
	if (mEnemyIndex == static_cast<int>(mEnemies.size()))
	{
		return;
	}
	mEnemies[mEnemyIndex]->Spawn();
	mEnemyIndex++;
}

void EnemySpawner::SpawnEnemyWave()
{
	for (int i = 0; i < ENEMY_WAVE_SIZE; i++)
	{
		SpawnEnemy();
	}
}

void EnemySpawner::Reset()
{
	for (auto& enemy : mEnemies)
	{
		enemy->Reset();
	}
	mEnemyIndex = 0;
}