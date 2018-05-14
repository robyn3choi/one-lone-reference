#include "Precompiled.h"
#include "CollisionHandler.h"


CollisionHandler::CollisionHandler()
{
}


CollisionHandler::~CollisionHandler()
{
}

void CollisionHandler::HandleCollisions(BulletPool* enemyBulletPool, BulletPool* playerBulletPool,
	std::vector<std::unique_ptr<Enemy>>* enemies, Player* player, Boss* boss)
{
	CheckIfPlayerWasHit(enemyBulletPool, player);
	CheckIfEnemiesWereHit(playerBulletPool, boss, enemies);
}

void CollisionHandler::CheckIfPlayerWasHit(BulletPool * enemyBulletPool, Player * player)
{
	for (const auto &bullet : enemyBulletPool->GetPool())
	{
		if (!bullet->IsActive())
		{
			continue;
		}
		SDL_Rect playerCollider = player->GetCollider();
		SDL_Rect bulletCollider = bullet->GetCollider();
		if (SDL_HasIntersection(&playerCollider, &bulletCollider))
		{
			if (!player->IsDashing())
			{
				player->TakeDamage();
				enemyBulletPool->ReturnBullet(bullet.get());
			}
		}
	}
}

void CollisionHandler::CheckIfEnemiesWereHit(BulletPool * playerBulletPool, Boss * boss, std::vector<std::unique_ptr<Enemy>>* enemies)
{
	for (const auto &bullet : playerBulletPool->GetPool())
	{
		if (!bullet->IsActive())
		{
			continue;
		}
		if (GameManager::Instance().HasReachedBoss())
		{
			SDL_Rect bossCollider = boss->GetCollider();
			SDL_Rect bulletCollider = bullet->GetCollider();
			if (SDL_HasIntersection(&bossCollider, &bulletCollider))
			{
				boss->TakeDamage();
				playerBulletPool->ReturnBullet(bullet.get());
			}
		}
		else
		{
			for (const auto& enemy : *enemies)
			{
				if (!enemy->IsActive())
				{
					continue;
				}
				SDL_Rect enemyCollider = enemy->GetCollider();
				SDL_Rect bulletCollider = bullet->GetCollider();
				if (SDL_HasIntersection(&enemyCollider, &bulletCollider))
				{
					enemy->TakeDamage();
					playerBulletPool->ReturnBullet(bullet.get());
				}
			}
		}
	}
}
