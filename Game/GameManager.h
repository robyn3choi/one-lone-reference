#pragma once
#include <vector>
#include "TextureManager.h"
#include "GameObject.h"
#include "BulletPool.h"
#include "Enemy.h"
#include "Boss.h"
#include "Player.h"
#include "EnemySpawner.h"
#include "DialogBoxHandler.h"
#include "SDLDeleters.h"

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
	void InitializeSDL();
	void CreateWindow();
	void CreateRenderer();
	void InitializeSDLImage();
	void CreateGameObjects();
	void Close();
	void Run();
	void HandleEnemyDeath();
	void HandlePlayerDeath();
	void Restart();
	void Win();
	void StopRunning();
	Player* const GetPlayer() const { return mPlayer.get(); }
	TextureManager* const GetTextureManager() const { return mTextureManager.get(); }
	SDL_Renderer* const GetRenderer() const { return mRenderer.get(); }
	bool HasReachedBoss() const { return mHasReachedBoss; }
	bool IsGameOver() const { return mIsGameOver; }
	bool HasWon() const { return mHasWon; }

private:
	GameManager();
	void UpdateAndRender(float& timeAtPreviousFrame, Vector2& cursorPos, DialogBoxHandler& dialogBoxHandler);
	void CenterCameraOverPlayer();
	void RenderTiledGround();
	float GetDeltaTime(float& timeAtPreviousFrame);
	void UpdateAndRenderGameObjects(float deltaTime);
	void RenderGameOverUI();
	void RenderPlayerHealth();
	void RenderBossHealth();
	void SetToInitialState();
	void SpawnBoss();

	std::unique_ptr<SDL_Window, SDLWindowDestroyer> mWindow = nullptr;
	std::unique_ptr<SDL_Renderer, SDLRendererDestroyer> mRenderer = nullptr;
	std::unique_ptr<TextureManager> mTextureManager = nullptr;
	SDL_Rect mCamera;

	std::vector<GameObject*> mGameObjects;
	std::unique_ptr<Player> mPlayer = nullptr;
	std::vector<std::unique_ptr<Enemy>> mEnemies;
	std::unique_ptr<BulletPool> mPlayerBulletPool = nullptr;
	std::unique_ptr<BulletPool> mEnemyBulletPool = nullptr;
	std::unique_ptr<EnemySpawner> mEnemySpawner = nullptr;
	std::unique_ptr<Boss> mBoss = nullptr;

	bool mIsGameRunning = false;
	bool mIsGameOver = false;
	bool mHasReachedBoss = false;
	bool mHasWon = false;
};

