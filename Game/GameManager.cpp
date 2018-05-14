#include "GameManager.h"
#include "Bullet.h"
#include "InputHandler.h"
#include "CollisionHandler.h"
#include <time.h>

GameManager::GameManager()
{
}

GameManager::~GameManager()
{
}

bool GameManager::Initialize()
{
	InitializeSDL();
	CreateWindow();
	CreateRenderer();
	InitializeSDLImage();

	SDL_SetRelativeMouseMode(SDL_TRUE);
	srand(static_cast<int>(time(NULL)));
	mTextureManager = std::make_unique<TextureManager>(mRenderer.get());
	mCamera = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };

	return true;
}

void GameManager::InitializeSDL()
{
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0)
	{
		throw SDLInitializationErrorException(SDL_GetError());
	}
}

void GameManager::CreateWindow()
{
	mWindow = std::unique_ptr<SDL_Window, SDLWindowDestroyer>(SDL_CreateWindow("One Lone Reference", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT,
		SDL_WINDOW_SHOWN | SDL_WINDOW_MAXIMIZED));

	if (mWindow == nullptr)
	{
		throw SDLWindowCreationErrorException(SDL_GetError());
	}
}

void GameManager::CreateRenderer()
{
	mRenderer = std::unique_ptr<SDL_Renderer, SDLRendererDestroyer>(SDL_CreateRenderer(mWindow.get(), -1,
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC));

	if (mRenderer == nullptr)
	{
		throw SDLRendererCreationErrorException(SDL_GetError());
	}
	SDL_SetRenderDrawColor(mRenderer.get(), 15, 15, 15, 255);
}

void GameManager::InitializeSDLImage()
{
	int imgFlags = IMG_INIT_PNG;
	if (!(IMG_Init(imgFlags) & imgFlags))
	{
		throw SDLImageInitializationErrorException(SDL_GetError());
	}
}


void GameManager::CreateGameObjects()
{
	mPlayerBulletPool = std::make_unique<BulletPool>(PLAYER_BULLET_POOL_SIZE, TextureType::PlayerBullet, PLAYER_BULLET_SPEED, false);
	mEnemyBulletPool = std::make_unique<BulletPool>(ENEMY_BULLET_POOL_SIZE,  TextureType::EnemyBullet, ENEMY_BULLET_SPEED, true);

	mPlayer = std::make_unique<Player>(TextureType::Player, mPlayerBulletPool.get());

	for (int i = 0; i < NUM_ENEMIES; i++)
	{
		auto enemy = std::make_unique<Enemy>(TextureType::Enemy, mPlayer.get(), mEnemyBulletPool.get());
		mEnemies.push_back(std::move(enemy));
		mEnemies[i]->SetActive(false);
	}

	mEnemySpawner = std::make_unique<EnemySpawner>(mEnemies);

	mBoss = std::make_unique<Boss>(TextureType::Boss, mPlayer.get(), mEnemyBulletPool.get());
	mBoss->SetActive(false);

	// put all gameObjects into mGameObjects
	decltype(auto) playerBulletPool = mPlayerBulletPool->GetPool();
	for (auto& bullet : playerBulletPool)
	{
		mGameObjects.push_back(bullet.get());
	}

	decltype(auto) enemyBulletPool = mEnemyBulletPool->GetPool();
	for (auto& bullet : enemyBulletPool)
	{
		mGameObjects.push_back(bullet.get());
	}

	mGameObjects.push_back(mPlayer.get());

	mGameObjects.push_back(mBoss.get());

	for (auto& enemy : mEnemies)
	{
		mGameObjects.push_back(enemy.get());
	}
}


void GameManager::Close()
{
	mTextureManager.reset();
	mEnemySpawner.reset();
	mEnemies.clear();
	mWindow.reset();
	mRenderer.reset();

	IMG_Quit();
	SDL_Quit();
}


void GameManager::Run()
{
	float timeAtPreviousFrame = 0;
	mIsGameRunning = true;
	InputHandler inputHandler;
	CollisionHandler collisionHandler;
	DialogBoxHandler dialogBoxHandler;

	SetToInitialState();

	while (mIsGameRunning)
	{
		inputHandler.HandleInput();

		collisionHandler.HandleCollisions(mEnemyBulletPool.get(), mPlayerBulletPool.get(), &mEnemies, mPlayer.get(), mBoss.get());

		Vector2 cursorPos = inputHandler.GetCursorPosition();
		UpdateAndRender(timeAtPreviousFrame, cursorPos, dialogBoxHandler);
	}

	Close();
}

void GameManager::SetToInitialState()
{
	Vector2 initialPos(LEVEL_WIDTH/2, LEVEL_HEIGHT/2);
	mPlayer->SetPosition(initialPos);
	mBoss->SetPosition(INITIAL_BOSS_POSITION);
	mEnemyBulletPool->Reset();
	mPlayerBulletPool->Reset();

	if (!mHasReachedBoss)
	{
		mEnemySpawner->SpawnInitialEnemies();
	}
}

void GameManager::CenterCameraOverPlayer()
{
	int playerWidth = mTextureManager->GetTexture(TextureType::Player)->GetWidth();
	int playerHeight = mTextureManager->GetTexture(TextureType::Player)->GetHeight();
	mCamera.x = (static_cast<int>(mPlayer->GetPosition().x) + playerWidth / 2) - SCREEN_WIDTH / 2;
	mCamera.y = (static_cast<int>(mPlayer->GetPosition().y) + playerHeight / 2) - SCREEN_HEIGHT / 2;
}

void GameManager::UpdateAndRender(float& timeAtPreviousFrame, Vector2& cursorPos, DialogBoxHandler& dialogBoxHandler)
{
	CenterCameraOverPlayer();

	SDL_RenderClear(mRenderer.get());

	RenderTiledGround();

	float deltaTime = GetDeltaTime(timeAtPreviousFrame);
	UpdateAndRenderGameObjects(deltaTime);

	if (mIsGameOver)
	{
		RenderGameOverUI();
	}

	mTextureManager->GetTexture(TextureType::Crosshair)->Render(static_cast<int>(cursorPos.x), static_cast<int>(cursorPos.y));

	RenderPlayerHealth();

	if (mHasReachedBoss)
	{
		RenderBossHealth();
		dialogBoxHandler.RunDialogBoxSequence(deltaTime);
	}

	SDL_RenderPresent(mRenderer.get());
}

void GameManager::RenderTiledGround()
{
	for (int i = 0; i <= LEVEL_WIDTH - TILE_WIDTH; i += TILE_WIDTH)
	{
		for (int j = 0; j <= LEVEL_HEIGHT - TILE_WIDTH; j += TILE_WIDTH)
		{
			mTextureManager->GetTexture(TextureType::Ground)->Render(i - mCamera.x, j - mCamera.y);
		}
	}
}

float GameManager::GetDeltaTime(float& timeAtPreviousFrame)
{
	float timeAtCurrentFrame = (float)SDL_GetTicks();
	float deltaTime = (timeAtCurrentFrame - timeAtPreviousFrame) / 1000; // deltatime in seconds
	timeAtPreviousFrame = timeAtCurrentFrame;
	return deltaTime;
}

void GameManager::UpdateAndRenderGameObjects(float deltaTime)
{
	for (auto& g : mGameObjects)
	{
		if (g->IsActive())
		{
			g->Update(deltaTime);
			Vector2 pos = g->GetPosition();
			Texture* tex = mTextureManager->GetTexture(g->GetTextureType());
			tex->Render(static_cast<int>(pos.x) - mCamera.x, static_cast<int>(pos.y) - mCamera.y);
		}
	}
}

void GameManager::RenderGameOverUI()
{
	Texture* gameOverText = mTextureManager->GetTexture(TextureType::GameOverText);
	gameOverText->Render(SCREEN_WIDTH / 2 - gameOverText->GetWidth() / 2, GAME_OVER_TEXT_Y);

	Texture* tryAgainBtn = mTextureManager->GetTexture(TextureType::TryAgainButton);
	tryAgainBtn->Render(SCREEN_WIDTH / 2 - tryAgainBtn->GetWidth() / 2, TRY_AGAIN_BUTTON_Y);
}

void GameManager::RenderPlayerHealth()
{
	Texture* heart = mTextureManager->GetTexture(TextureType::Heart);
	int currentHealth = mPlayer->GetHealth();

	if (currentHealth > 0)
	{
		heart->Render(50, 50);
	}
	if (currentHealth > 1)
	{
		heart->Render(120, 50);
	}
	if (currentHealth > 2)
	{
		heart->Render(190, 50);
	}
}

void GameManager::RenderBossHealth()
{
	Texture* healthBarBackground = mTextureManager->GetTexture(TextureType::BossHealthBackground);
	healthBarBackground->SetWidth(BOSS_HEALTH_BAR_WIDTH);
	healthBarBackground->SetHeight(BOSS_HEALTH_BAR_HEIGHT);
	healthBarBackground->Render(SCREEN_WIDTH / 2 - BOSS_HEALTH_BAR_WIDTH / 2, BOSS_HEALTH_BAR_Y);

	int currentHealth = mBoss->GetHealth();
	float percentage = static_cast<float>(currentHealth) / static_cast<float>(BOSS_HEALTH);

	Texture* healthBarFill = mTextureManager->GetTexture(TextureType::BossHealthFill);
	healthBarFill->SetWidth(percentage * static_cast<float>(BOSS_HEALTH_BAR_WIDTH));
	healthBarFill->SetHeight(BOSS_HEALTH_BAR_HEIGHT);
	healthBarFill->Render(SCREEN_WIDTH / 2 - BOSS_HEALTH_BAR_WIDTH / 2, BOSS_HEALTH_BAR_Y);
}

void GameManager::HandleEnemyDeath()
{
	bool areAnyEnemiesActive = false;
	for (auto& enemy : mEnemies)
	{
		if (enemy->IsActive())
		{
			areAnyEnemiesActive = true;
			break;
		}
	}

	if (areAnyEnemiesActive || mEnemySpawner->AreThereEnemiesLeft())
	{
		mEnemySpawner->SpawnEnemyWave();
	}
	else
	{
		SpawnBoss();
	}
}

void GameManager::SpawnBoss()
{
	mBoss->SetPosition(Vector2(static_cast<int>(LEVEL_WIDTH/2), static_cast<int>(LEVEL_HEIGHT/2)));
	mBoss->SetActive(true);
	mHasReachedBoss = true;
}

void GameManager::HandlePlayerDeath()
{
	mIsGameOver = true;
	mEnemySpawner->Reset();
}

void GameManager::Restart()
{
	mIsGameOver = false;
	mPlayer->Reset();
	mBoss->Reset();
	mEnemySpawner->Reset();
	SetToInitialState();
}

void GameManager::Win()
{
	mHasWon = true;
}

void GameManager::StopRunning()
{
	mIsGameRunning = false;
}
