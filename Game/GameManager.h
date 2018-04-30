#pragma once
#include <vector>
#include "TextureManager.h"
#include "GameObject.h"
#include "BulletPool.h"
#include "Enemy.h"
#include "Player.h"
#include "EnemySpawner.h"
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
	TextureManager* const GetTextureManager();
	bool IsOutOfBounds(Vector2 pos, float width, float height);

private:
	GameManager();
	void HandleInput(SDL_Event& e);
	void CheckCollisions();
	void KeepCameraInBounds();
	void SetToInitialState();
	void CenterCameraOverPlayer(float playerWidth, float playerHeight);
	void RenderTiledGround();
	float GetDeltaTime(float& timeAtPreviousFrame);
	void UpdateAndRenderGameObjects(float deltaTime);
	void RenderGameOverUI();

	std::unique_ptr<SDL_Window, SDLWindowDestroyer> mWindow = nullptr;
	std::unique_ptr<SDL_Renderer, SDLRendererDestroyer> mRenderer = nullptr;
	std::unique_ptr<TextureManager> mTextureManager = nullptr;
	SDL_Rect mCamera;

	std::vector<std::unique_ptr<GameObject>> mGameObjects;
	std::unique_ptr<Player> mPlayer = nullptr;
	std::vector<std::unique_ptr<Enemy>> mEnemies;
	std::unique_ptr<BulletPool> mPlayerBulletPool = nullptr;
	std::unique_ptr<BulletPool> mEnemyBulletPool = nullptr;
	std::unique_ptr<EnemySpawner> mEnemySpawner = nullptr;

	bool mIsGameRunning;
	bool mIsGameOver = false;
	Vector2 m_CursorPos;

	std::unique_ptr<Texture> mTryAgainButton;
	Texture* mGroundTexture = nullptr;
};

