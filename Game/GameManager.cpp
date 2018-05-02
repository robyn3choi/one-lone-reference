#include "GameManager.h"
#include "Bullet.h"
#include "BulletPool.h"
#include "Exceptions.h"
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

	// put all gameObjects into mGameObjects
	mGameObjects.push_back(mPlayer.get());

	for (auto& enemy : mEnemies)
	{
		mGameObjects.push_back(enemy.get());
	}

	auto playerBulletPool = mPlayerBulletPool->GetPool();
	for (auto& bullet : playerBulletPool)
	{
		mGameObjects.push_back(bullet.get());
	}

	auto enemyBulletPool = mEnemyBulletPool->GetPool();
	for (auto& bullet : enemyBulletPool)
	{
		mGameObjects.push_back(bullet.get());
	}

	// environment
	mGroundTexture = mTextureManager->GetTexture(TextureType::Ground);
}


void GameManager::Close()
{
	mTextureManager.reset();
	mEnemySpawner.reset();
	mEnemies.clear();

	//for(auto& g : mGameObjects)
	//{
	//	g.reset();
	//}

	//SDL_DestroyRenderer(mRenderer.get());
	//SDL_DestroyWindow(mWindow.get());
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

	int playerWidth = mTextureManager->GetTexture(TextureType::Player)->GetWidth();
	int playerHeight = mTextureManager->GetTexture(TextureType::Player)->GetHeight();

	while (mIsGameRunning)
	{
		HandleInput(e);

		CenterCameraOverPlayer(playerWidth, playerHeight);

		CheckCollisions();
		
		SDL_RenderClear(mRenderer.get());

		RenderTiledGround();

		float deltaTime = GetDeltaTime(timeAtPreviousFrame);
		UpdateAndRenderGameObjects(deltaTime);

		if (mIsGameOver)
		{
			RenderGameOverUI();
		}

		//mTextureManager->GetTexture(TextureType::Cursor)->Render(m_CursorPos.x - mCamera.x, m_CursorPos.y - mCamera.y);

		SDL_RenderPresent(mRenderer.get());
	}

	Close();
}

void GameManager::SetToInitialState()
{
	Vector2 initialPos(20, 1);
	mPlayer->SetPosition(initialPos);
	mEnemySpawner->SpawnInitialEnemies();
}

void GameManager::CenterCameraOverPlayer(int playerWidth, int playerHeight)
{
	mCamera.x = (static_cast<int>(mPlayer->GetPosition().x) + playerWidth / 2) - SCREEN_WIDTH / 2;
	mCamera.y = (static_cast<int>(mPlayer->GetPosition().y) + playerHeight / 2) - SCREEN_HEIGHT / 2;
}

void GameManager::RenderTiledGround()
{
	for (int i = 0; i <= LEVEL_WIDTH - TILE_WIDTH; i += TILE_WIDTH)
	{
		for (int j = 0; j <= LEVEL_HEIGHT - TILE_WIDTH; j += TILE_WIDTH)
		{
			mGroundTexture->Render(i - mCamera.x, j - mCamera.y);
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
	mTryAgainButton->Render(SCREEN_WIDTH / 2 - mTryAgainButton->GetWidth() / 2, 700);
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

TextureManager* const GameManager::GetTextureManager()
{
	return mTextureManager.get();
}

void GameManager::HandleInput(SDL_Event& e)
{
	int mouseX;
	int mouseY;
	SDL_GetMouseState(&mouseX, &mouseY);
	Vector2 mousePos(static_cast<float>(mouseX), static_cast<float>(mouseY));
	Vector2 mouseDir(mousePos - Vector2(SCREEN_WIDTH/2, SCREEN_HEIGHT/2));
	mouseDir.Normalize();
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

	Vector2 centrePos = Vector2(static_cast<float>(mCamera.x + SCREEN_WIDTH / 2), static_cast<float>(mCamera.y + SCREEN_HEIGHT / 2));
	m_CursorPos = centrePos + mouseDir * CURSOR_RADIUS;

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
				mPlayerBulletPool->GetBullet()->Shoot(m_CursorPos, mouseDir);
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
					int tryAgainBtnX = SCREEN_WIDTH / 2 - mTryAgainButton->GetWidth() / 2;
					int tryAgainBtnY = 700;
					//If the mouse is over the button 
					if ((mouseX > tryAgainBtnX) &&
						(mouseX <tryAgainBtnX + mTryAgainButton->GetWidth()) &&
						(mouseY > tryAgainBtnY) &&
						(mouseY < tryAgainBtnY + mTryAgainButton->GetHeight()))
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
	int enemyBulletTexWidth = mTextureManager->GetTexture(TextureType::EnemyBullet)->GetWidth();
	int enemyBulletTexHeight = mTextureManager->GetTexture(TextureType::EnemyBullet)->GetHeight();
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
				mEnemyBulletPool->ReturnBullet(bullet.get());
				std::cout << "player damaged" << std::endl;
			}
		}
		else
		{
			if (IsOutOfBounds(bullet->GetPosition(), enemyBulletTexWidth, enemyBulletTexHeight))
			{
				mEnemyBulletPool->ReturnBullet(bullet.get());
			}
		}
	}

	int playerBulletTexWidth = mTextureManager->GetTexture(TextureType::PlayerBullet)->GetWidth();
	int playerBulletTexHeight = mTextureManager->GetTexture(TextureType::PlayerBullet)->GetHeight();
	// check if enemies have been hit by player bullets
	// check if bullets have hit walls
	for (auto &bullet : mPlayerBulletPool->GetPool())
	{
		for (auto& enemy : mEnemies)
		{
			if (!bullet->IsActive())
			{
				continue;
			}
			SDL_Rect enemyCollider = enemy->GetCollider();
			SDL_Rect bulletCollider = bullet->GetCollider();
			if (SDL_HasIntersection(&enemyCollider, &bulletCollider))
			{
				enemy->TakeDamage();
				mPlayerBulletPool->ReturnBullet(bullet.get());
				std::cout << "enemy damaged" << std::endl;
			}
			else
			{
				if (IsOutOfBounds(bullet->GetPosition(), playerBulletTexWidth, playerBulletTexHeight))
				{
					mEnemyBulletPool->ReturnBullet(bullet.get());
				}
			}
		}
	}
}

bool GameManager::IsOutOfBounds(Vector2 pos, int width, int height)
{
	if (static_cast<int>(pos.x) > LEVEL_WIDTH - width ||
		static_cast<int>(pos.x) < 0 ||
		static_cast<int>(pos.y) > LEVEL_HEIGHT - height ||
		static_cast<int>(pos.y) < 0)
	{
		return true;
	}
	return false;
}

void GameManager::KeepCameraInBounds()
{
	//Keep the camera in bounds
	if (mCamera.x < 0)
	{
		mCamera.x = 0;
	}
	if (mCamera.y < 0)
	{
		mCamera.y = 0;
	}
	if (mCamera.x > LEVEL_WIDTH - mCamera.w)
	{
		mCamera.x = LEVEL_WIDTH - mCamera.w;
	}
	if (mCamera.y > LEVEL_HEIGHT - mCamera.h)
	{
		mCamera.y = LEVEL_HEIGHT - mCamera.h;
	}
}
