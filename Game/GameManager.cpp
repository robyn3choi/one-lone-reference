#include "GameManager.h"
#include "Bullet.h"
#include "BulletPool.h"
#include "Exceptions.h"
#include "GameLoop.h"
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

	//SDL_SetRelativeMouseMode(SDL_TRUE);
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
	mPlayer = std::make_unique<Player>(TextureType::Player);

	mPlayerBulletPool = std::make_unique<BulletPool>(PLAYER_BULLET_POOL_SIZE, TextureType::PlayerBullet, PLAYER_BULLET_SPEED, false);
	mEnemyBulletPool = std::make_unique<BulletPool>(ENEMY_BULLET_POOL_SIZE,  TextureType::EnemyBullet, ENEMY_BULLET_SPEED, true);

	for (int i = 0; i < NUM_ENEMIES; i++)
	{
		auto enemy = std::make_unique<Enemy>(TextureType::Enemy, mPlayer.get(), mEnemyBulletPool.get());
		mEnemies.push_back(std::move(enemy));
		mEnemies[i]->SetActive(false);
	}

	mEnemySpawner = std::make_unique<EnemySpawner>(mEnemies);

	mBoss = std::make_unique<Boss>(TextureType::Enemy, mPlayer.get(), mEnemyBulletPool.get());

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

	//Event handler
	SDL_Event e;

	SetToInitialState();

	mBoss->SetActive(true);
	mBoss->SetPosition(Vector2(100, 100));

	while (mIsGameRunning)
	{
		HandleInput(e);

		CenterCameraOverPlayer();

		CheckCollisions();

		SDL_RenderClear(mRenderer.get());

		RenderTiledGround();

		float deltaTime = GetDeltaTime(timeAtPreviousFrame);
		UpdateAndRenderGameObjects(deltaTime);

		if (mIsGameOver)
		{
			RenderGameOverUI();
		}

		mTextureManager->GetTexture(TextureType::Crosshair)->Render(static_cast<int>(m_CursorPos.x), static_cast<int>(m_CursorPos.y));

		RenderHearts();

		SDL_RenderPresent(mRenderer.get());
	}

	Close();
}

void GameManager::SetToInitialState()
{
	Vector2 initialPos(LEVEL_WIDTH/2, LEVEL_HEIGHT/2);
	mPlayer->SetPosition(initialPos);
	//mEnemySpawner->SpawnInitialEnemies();
}

void GameManager::CenterCameraOverPlayer()
{
	int playerWidth = mTextureManager->GetTexture(TextureType::Player)->GetWidth();
	int playerHeight = mTextureManager->GetTexture(TextureType::Player)->GetHeight();
	mCamera.x = (static_cast<int>(mPlayer->GetPosition().x) + playerWidth / 2) - SCREEN_WIDTH / 2;
	mCamera.y = (static_cast<int>(mPlayer->GetPosition().y) + playerHeight / 2) - SCREEN_HEIGHT / 2;
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
	gameOverText->Render(SCREEN_WIDTH / 2 - gameOverText->GetWidth() / 2, 200);

	// TODO: REFACTOR
	Texture* tryAgainBtn = mTextureManager->GetTexture(TextureType::TryAgainButton);
	tryAgainBtn->Render(SCREEN_WIDTH / 2 - tryAgainBtn->GetWidth() / 2, 700);
}

void GameManager::RenderHearts()
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

void GameManager::HandleEnemyDeath()
{
	mEnemySpawner->SpawnEnemy();
	mEnemySpawner->SpawnEnemy();
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
	mEnemySpawner->Reset();
	SetToInitialState();
}

void GameManager::Win()
{
}

void GameManager::HandleInput(SDL_Event& e)
{
	Vector2 moveDir(0, 0);
	const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
	if (currentKeyStates[SDL_SCANCODE_W])
	{
		moveDir += Vector2(0, -1);
	}
	else if (currentKeyStates[SDL_SCANCODE_S])
	{
		moveDir += Vector2(0, 1);
	}
	if (currentKeyStates[SDL_SCANCODE_A])
	{
		moveDir += Vector2(-1, 0);
	}
	else if (currentKeyStates[SDL_SCANCODE_D])
	{
		moveDir += Vector2(1, 0);
	}

	mPlayer->Move(moveDir);

	int mouseX;
	int mouseY;
	SDL_GetMouseState(&mouseX, &mouseY);
	Vector2 mousePos(static_cast<float>(mouseX), static_cast<float>(mouseY));
	Vector2 mouseDir(mousePos - Vector2(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2));
	mouseDir.Normalize();
	
	m_CursorPos = mousePos; // + mouseDir * CURSOR_RADIUS;

	//Handle events on queue
	while (SDL_PollEvent(&e) != 0)
	{
		if (e.type == SDL_QUIT)
		{
			mIsGameRunning = false;
		}
		
		if (e.type == SDL_MOUSEBUTTONDOWN) 
		{
			if (e.button.button == SDL_BUTTON_LEFT)
			{
				// shoot
				//Bullet* bullet = mPlayerBulletPool->GetBullet();
				mPlayerBulletPool->GetBullet()->Shoot(mPlayer->GetPosition(), mouseDir);
			}
			if (e.button.button == SDL_BUTTON_RIGHT)
			{
				//dash
				if (mPlayer->IsDashing())
				{
					return;
				}
				if (moveDir.GetLength() == 0)
				{
					mPlayer->Dash(mouseDir);
				}
				else
				{
					mPlayer->Dash(moveDir);
				}
			}
		}

		if (mIsGameOver)
		{
			if (e.type == SDL_MOUSEBUTTONUP)
			{
				if (e.button.button == SDL_BUTTON_LEFT)
				{
					Texture* tryAgainBtn = mTextureManager->GetTexture(TextureType::TryAgainButton);
					int tryAgainBtnX = SCREEN_WIDTH / 2 - tryAgainBtn->GetWidth() / 2;
					int tryAgainBtnY = 700;
					//If the mouse is over the button 
					if ((mouseX > tryAgainBtnX) &&
						(mouseX <tryAgainBtnX + tryAgainBtn->GetWidth()) &&
						(mouseY > tryAgainBtnY) &&
						(mouseY < tryAgainBtnY + tryAgainBtn->GetHeight()))
					{
						//Set the button sprite 
						Restart();
					}
				}
			}
		}
		
	}
}

void GameManager::CheckCollisions()
{
	// check if player has been hit by enemy bullets 
	// check if bullets have hit walls
	for (auto &bullet : mEnemyBulletPool->GetPool())
	{
		if (!bullet->IsActive())
		{
			continue;
		}
		SDL_Rect playerCollider = mPlayer->GetCollider();
		SDL_Rect bulletCollider = bullet->GetCollider();
		if (SDL_HasIntersection(&playerCollider, &bulletCollider))
		{
			if (!mPlayer->IsDashing())
			{
				mPlayer->TakeDamage();
				bullet->SetActive(false);
			}
		}
	}

	// check if enemies have been hit by player bullets
	// check if bullets have hit walls
	for (auto &bullet : mPlayerBulletPool->GetPool())
	{
		if (!bullet->IsActive())
		{
			continue;
		}
		for (auto& enemy : mEnemies)
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
				bullet->SetActive(false);
				std::cout << "enemy damaged" << std::endl;
			}
		}
	}
}
