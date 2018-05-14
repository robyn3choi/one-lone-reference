#pragma once
class CollisionHandler
{
public:
	CollisionHandler();
	~CollisionHandler();

	void HandleCollisions(BulletPool* enemyBulletPool, BulletPool* playerBulletPool, 
		std::vector<std::unique_ptr<Enemy>>* enemies, Player* player, Boss* boss);

private:
	void CheckIfPlayerWasHit(BulletPool* enemyBulletPool, Player* player);
	void CheckIfEnemiesWereHit(BulletPool* playerBulletPool, Boss* boss, std::vector<std::unique_ptr<Enemy>>* enemies);
};

