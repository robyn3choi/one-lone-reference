#include "GameManager.h"
#include "Bullet.h"
#include "BulletPool.h"
#include <time.h>


GameManager::GameManager()
{
}


GameManager::~GameManager()
{
}

bool GameManager::Initialize()
{
	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0)
	{
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		return false;
	}

	//Create window
	mWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_MAXIMIZED);
	if (mWindow == NULL)
	{
		printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
		return false;
	}

	//Create renderer for window
	mRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED);
	if (mRenderer == NULL)
	{
		printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
		return false;
	}

	//Initialize renderer color
	SDL_SetRenderDrawColor(mRenderer, 15, 15, 15, 255);

	//Initialize PNG loading
	int imgFlags = IMG_INIT_PNG;
	if (!(IMG_Init(imgFlags) & imgFlags))
	{
		printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
		return false;
	}

	srand(time(NULL));
	mTextureManager = new TextureManager(mRenderer);
	mCamera = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };

	return true;
}

void GameManager::CreateGameObjects()
{
	mPlayer = new Player(TextureType::Player);

	mPlayerBulletPool = new BulletPool(PLAYER_BULLET_POOL_SIZE, TextureType::PlayerBullet, PLAYER_BULLET_SPEED, false);
	mEnemyBulletPool = new BulletPool(ENEMY_BULLET_POOL_SIZE,  TextureType::EnemyBullet, ENEMY_BULLET_SPEED, true);

	for (int i = 0; i < NUM_ENEMIES; i++)
	{
		mEnemies.push_back(new Enemy(TextureType::Enemy, mPlayer, mEnemyBulletPool));
		mEnemies[i]->SetActive(false);
	}

	mEnemySpawner = new EnemySpawner(mEnemies);

	// put all gameObjects into mGameObjects
	mGameObjects.push_back(mPlayer);
	mGameObjects.insert(mGameObjects.end(), mEnemies.begin(), mEnemies.end());

	auto pBulletPool = mPlayerBulletPool->GetPool();
	mGameObjects.insert(mGameObjects.end(), pBulletPool.begin(), pBulletPool.end());

	auto eBulletPool = mEnemyBulletPool->GetPool();
	mGameObjects.insert(mGameObjects.end(), eBulletPool.begin(), eBulletPool.end());

	// environment
	mGroundTexture = mTextureManager->GetTexture(TextureType::Ground);
}


void GameManager::Close()
{
	mPlayer = nullptr;
	delete mTextureManager;
	mTextureManager = nullptr;
	delete mPlayerBulletPool;
	mPlayerBulletPool = nullptr;
	delete mEnemyBulletPool;
	mEnemyBulletPool = nullptr;
	delete mTextureManager;
	mTextureManager = nullptr;
	delete mEnemySpawner;
	mEnemySpawner = nullptr;

	mEnemies.clear();

	for(GameObject* g : mGameObjects)
	{
		delete g;
		g = nullptr;
	}

	//Destroy window	
	SDL_DestroyRenderer(mRenderer);
	SDL_DestroyWindow(mWindow);
	mWindow = nullptr;
	mRenderer = nullptr;

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}

void GameManager::Run()
{
	float last = 0;

	mIsGameRunning = true;

	//Event handler
	SDL_Event e;

	mPlayer->SetPosition(Vector2(20, 1));

	mEnemySpawner->SpawnInitialEnemies();

	float playerWidth = mTextureManager->GetTexture(TextureType::Player)->GetWidth();
	float playerHeight = mTextureManager->GetTexture(TextureType::Player)->GetHeight();

	//While application is running
	while (mIsGameRunning)
	{
		HandleInput(e);

		//Center the camera over the player
		mCamera.x = (mPlayer->GetPosition().x + playerWidth / 2) - SCREEN_WIDTH / 2;
		mCamera.y = (mPlayer->GetPosition().y + playerHeight / 2) - SCREEN_HEIGHT / 2;

		CheckCollisions();
		
		//Clear screen
		SDL_RenderClear(mRenderer);

		// Get deltaTime
		float now = (float) SDL_GetTicks();
		float deltaTime = (now - last) / 1000; // deltatime in seconds
		last = now;

		// render ground
		for (int i = 0; i <= LEVEL_WIDTH - TILE_WIDTH; i += TILE_WIDTH)
		{
			for (int j = 0; j <= LEVEL_HEIGHT - TILE_WIDTH; j += TILE_WIDTH)
			{
				mGroundTexture->Render(i - mCamera.x, j - mCamera.y);
			}
		}

		// update and render gameobjects
		for (GameObject*& g : mGameObjects)
		{
			if (g->IsActive())
			{
				g->Update(deltaTime);
				Vector2 pos = g->GetPosition();
				Texture* tex = mTextureManager->GetTexture(g->GetTextureType());
				tex->Render(pos.x - mCamera.x, pos.y - mCamera.y);
			}
		}

		//Update screen
		SDL_RenderPresent(mRenderer);
	}

	//Free resources and close SDL
	Close();
}

void GameManager::HandleEnemyDeath()
{
	mEnemySpawner->SpawnEnemy();
}

void GameManager::HandlePlayerDeath()
{
}

TextureManager * GameManager::GetTextureManager()
{
	return mTextureManager;
}

void GameManager::HandleInput(SDL_Event& e)
{
	int mouseX;
	int mouseY;
	SDL_GetMouseState(&mouseX, &mouseY);
	Vector2 mousePos(mouseX, mouseY);
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

	//Handle events on queue
	while (SDL_PollEvent(&e) != 0)
	{
		if (e.type == SDL_QUIT)
		{
			mIsGameRunning = false;
		}
		// shoot
		if (e.type == SDL_MOUSEBUTTONDOWN) {
			Bullet* bullet = mPlayerBulletPool->GetBullet();
			bullet->Shoot(mPlayer->GetPosition(), mouseDir);
		}
		//dash
		if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_LSHIFT)
		{
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
}

void GameManager::CheckCollisions()
{
	float enemyBulletTexWidth = mTextureManager->GetTexture(TextureType::EnemyBullet)->GetWidth();
	float enemyBulletTexHeight = mTextureManager->GetTexture(TextureType::EnemyBullet)->GetHeight();
	// check if player has been hit by enemy bullets 
	// check if bullets have hit walls
	for (Bullet* const &bullet : mEnemyBulletPool->GetPool())
	{
		if (!bullet->IsActive())
		{
			continue;
		}
		if (SDL_HasIntersection(&mPlayer->GetCollider(), &bullet->GetCollider()))
		{
			if (!mPlayer->IsDashing())
			{
				mPlayer->TakeDamage();
				mEnemyBulletPool->ReturnBullet(bullet);
				std::cout << "player damaged" << std::endl;
			}
		}
		else
		{
			if (IsOutOfBounds(bullet->GetPosition(), enemyBulletTexWidth, enemyBulletTexHeight))
			{
				mEnemyBulletPool->ReturnBullet(bullet);
			}
		}
	}

	float playerBulletTexWidth = mTextureManager->GetTexture(TextureType::PlayerBullet)->GetWidth();
	float playerBulletTexHeight = mTextureManager->GetTexture(TextureType::PlayerBullet)->GetHeight();
	// check if enemies have been hit by player bullets
	// check if bullets have hit walls
	for (Bullet* const &bullet : mPlayerBulletPool->GetPool())
	{
		for (Enemy*& enemy : mEnemies)
		{
			if (!bullet->IsActive())
			{
				continue;
			}
			if (SDL_HasIntersection(&enemy->GetCollider(), &bullet->GetCollider()))
			{
				enemy->TakeDamage();
				mPlayerBulletPool->ReturnBullet(bullet);
				std::cout << "enemy damaged" << std::endl;
			}
			else
			{
				if (IsOutOfBounds(bullet->GetPosition(), playerBulletTexWidth, playerBulletTexHeight))
				{
					mEnemyBulletPool->ReturnBullet(bullet);
				}
			}
		}
	}
}

bool GameManager::IsOutOfBounds(Vector2 pos, float width, float height)
{
	if (pos.x > LEVEL_WIDTH - width ||
		pos.x < 0 ||
		pos.y > LEVEL_HEIGHT - height ||
		pos.y < 0)
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
