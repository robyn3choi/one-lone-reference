#pragma once
#include <vector>
#include "GameObject.h"
#include "BulletPool.h"
#include "Enemy.h"
#include "Player.h"
#include "EnemySpawner.h"

class GameManager
{
public:
	GameManager();
	~GameManager();

	bool Initialize();
	void CreateGameObjects();

	void Close();
	void Run();

private:
	void HandleInput(SDL_Event& e);
	void CheckCollisions();
	void KeepCameraInBounds();

	SDL_Window* mWindow;
	SDL_Renderer* mRenderer;
	TextureManager* mTextureManager;
	SDL_Rect mCamera;
	std::vector<GameObject*> mGameObjects;

	Player* mPlayer;
	std::vector<Enemy*> mEnemies;
	BulletPool* mPlayerBulletPool;
	BulletPool* mEnemyBulletPool;
	EnemySpawner* mEnemySpawner;

	bool mIsGameRunning;
};

