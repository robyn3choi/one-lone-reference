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
	void SpawnEnemy();
	void SpawnEnemyWave();
	bool AreThereEnemiesLeft() const { return (mEnemyIndex < mEnemies.size()); }
	void Reset();

private:
	std::vector<std::unique_ptr<Enemy>>& mEnemies;
	int mEnemyIndex = 0;
};

