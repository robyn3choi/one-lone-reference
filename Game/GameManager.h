#pragma once
#include <vector>
#include "TextureManager.h"
#include "GameObject.h"
#include "BulletPool.h"
#include "Enemy.h"
#include "Player.h"
#include "EnemySpawner.h"

class GameManager
{
public:
	~GameManager();

	static GameManager& Instance()
	{
		static GameManager *instance = new GameManager();
		return *instance;
	}


	bool Initialize();
	void CreateGameObjects();
	void Close();
	void Run();
	void HandleEnemyDeath();
	void HandlePlayerDeath();
	TextureManager* GetTextureManager();
	TTF_Font* GetFont();
	bool IsOutOfBounds(Vector2 pos, float width, float height);

private:
	GameManager();
	void HandleInput(SDL_Event& e);
	void CheckCollisions();
	void KeepCameraInBounds();

	SDL_Window* mWindow = nullptr;
	SDL_Renderer* mRenderer = nullptr;
	TTF_Font* mFont = nullptr;
	TextureManager* mTextureManager = nullptr;
	SDL_Rect mCamera;
	std::vector<GameObject*> mGameObjects;
	Texture* mGroundTexture = nullptr;

	Player* mPlayer = nullptr;
	std::vector<Enemy*> mEnemies;
	BulletPool* mPlayerBulletPool = nullptr;
	BulletPool* mEnemyBulletPool = nullptr;
	EnemySpawner* mEnemySpawner = nullptr;

	bool mIsGameRunning;
	Vector2 m_CursorPos;
};

