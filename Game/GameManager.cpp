#include "GameManager.h"
#include "Bullet.h"
#include "BulletPool.h"


GameManager::GameManager()
{
}


GameManager::~GameManager()
{
}

bool GameManager::Initialize()
{
	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		return false;
	}

	//Create window
	mWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
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
	SDL_SetRenderDrawColor(mRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

	//Initialize PNG loading
	int imgFlags = IMG_INIT_PNG;
	if (!(IMG_Init(imgFlags) & imgFlags))
	{
		printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
		return false;
	}

	mTextureManager = new TextureManager(mRenderer);
	mCamera = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };

	return true;
}

void GameManager::CreateGameObjects()
{
	mPlayer = new Player(TextureType::Player);

	for (int i = 0; i < NUM_ENEMIES; i++)
	{
		mEnemies.push_back(new Enemy(TextureType::Enemy));
	}

	mPlayerBulletPool = new BulletPool(PLAYER_BULLET_POOL_SIZE, TextureType::PlayerBullet, PLAYER_BULLET_SPEED, false);
	mEnemyBulletPool = new BulletPool(ENEMY_BULLET_POOL_SIZE,  TextureType::EnemyBullet, ENEMY_BULLET_SPEED, true);

	// put all gameObjects into mGameObjects
	mGameObjects.push_back(mPlayer);
	mGameObjects.insert(mGameObjects.end(), mEnemies.begin(), mEnemies.end());

	auto pBulletPool = mPlayerBulletPool->GetPool();
	mGameObjects.insert(mGameObjects.end(), pBulletPool.begin(), pBulletPool.end());

	auto eBulletPool = mEnemyBulletPool->GetPool();
	mGameObjects.insert(mGameObjects.end(), eBulletPool.begin(), eBulletPool.end());
}


void GameManager::Close()
{
	delete mTextureManager;
	mTextureManager = nullptr;
	delete mPlayer;
	mPlayer = nullptr;
	delete mPlayerBulletPool;
	mPlayerBulletPool = nullptr;
	delete mEnemyBulletPool;
	mEnemyBulletPool = nullptr;
	delete mTextureManager;
	mTextureManager = nullptr;

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

	mEnemies[0]->SetPosition(Vector2(300, 1));
	mEnemies[1]->SetPosition(Vector2(400, 1));
	mEnemies[2]->SetPosition(Vector2(400, 1));

	float playerWidth = TextureManager::GetTexture(TextureType::Player)->GetWidth();
	float playerHeight = TextureManager::GetTexture(TextureType::Player)->GetHeight();

	//While application is running
	while (mIsGameRunning)
	{
		HandleInput(e);

		//Center the camera over the dot
		mCamera.x = (mPlayer->GetPosition().x + playerWidth / 2) - SCREEN_WIDTH / 2;
		mCamera.y = (mPlayer->GetPosition().y + playerHeight / 2) - SCREEN_HEIGHT / 2;

		KeepCameraInBounds();

		CheckCollisions();

		
		//Clear screen
		SDL_SetRenderDrawColor(mRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
		SDL_RenderClear(mRenderer);

		// render background
		TextureManager::GetTexture(TextureType::Background)->Render(-mCamera.x, -mCamera.y);

		// Get deltaTime
		float now = (float) SDL_GetTicks();
		float deltaTime = (now - last) / 1000; // deltatime in seconds
		last = now;

		for (GameObject*& g : mGameObjects)
		{
			if (g->IsActive())
			{
				g->Update(deltaTime);
				g->Render(mCamera);
			}
		}

		//Update screen
		SDL_RenderPresent(mRenderer);
	}

	//Free resources and close SDL
	Close();
}

void GameManager::HandleInput(SDL_Event& e)
{
	int mouseX;
	int mouseY;
	SDL_GetMouseState(&mouseX, &mouseY);
	Vector2 mousePos(mouseX, mouseY);
	Vector2 mouseDir(mousePos - mPlayer->GetPosition());
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
	// check if player has been hit by enemy bullets
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
				// TODO: damage player
				mEnemyBulletPool->ReturnBullet(bullet);
				std::cout << "player damaged" << std::endl;
			}
		}
	}

	// check if enemies have been hit by player bullets
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
				// TODO: damage enemy
				mPlayerBulletPool->ReturnBullet(bullet);
				std::cout << "enemy damaged" << std::endl;
			}
		}
	}
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
