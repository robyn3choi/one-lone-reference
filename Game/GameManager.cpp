#include "GameManager.h"
#include "Player.h"
#include "Bullet.h"


GameManager::GameManager() :
	mPlayerTexture(mRenderer),
	mBulletTexture(mRenderer),
	mBackgroundTexture(mRenderer)
{
}


GameManager::~GameManager()
{
}

bool GameManager::Initialize()
{
	bool success = true;

	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		success = false;
	}
	else
	{
		//Create window
		mWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (mWindow == NULL)
		{
			printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
			success = false;
		}
		else
		{
			//Create renderer for window
			mRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED);
			if (mRenderer == NULL)
			{
				printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
				success = false;
			}
			else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor(mRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if (!(IMG_Init(imgFlags) & imgFlags))
				{
					printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
					success = false;
				}
			}
		}
	}
	return success;
}

bool GameManager::LoadMedia()
{
	bool success = true;

	if (!mPlayerTexture.LoadFromFile("Images/testImage.png"))
	{
		printf("Failed to load player texture!\n");
		success = false;
	}

	if (!mBulletTexture.LoadFromFile("Images/cursor.png"))
	{
		printf("Failed to load bullet texture!\n");
		success = false;
	}

	if (!mBackgroundTexture.LoadFromFile("Images/bg.png"))
	{
		printf("Failed to load background texture!\n");
		success = false;
	}

	return success;
}

void GameManager::Close()
{
	//Free loaded images
	mPlayerTexture.Free();
	mBulletTexture.Free();
	mBackgroundTexture.Free();

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
	Uint32 last = 0;

	//Main loop flag
	bool quit = false;

	//Event handler
	SDL_Event e;

	mCamera = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	SDL_Rect playerCollider = { 0, 0, mPlayerTexture.GetWidth(), mPlayerTexture.GetHeight() };
	Player* player = new Player(&mPlayerTexture, mCamera, playerCollider, 1);
	player->SetPosition(Vector2(20, 1));

	SDL_Rect bulletCollider = { 0, 0, mBulletTexture.GetWidth(), mBulletTexture.GetHeight() };
	Bullet* bullet = new Bullet(&mBulletTexture, mCamera, bulletCollider);
	bullet->SetActive(false);

	// enemy for now
	SDL_Rect enemyCollider = { 0, 0, mPlayerTexture.GetWidth(), mPlayerTexture.GetHeight() };
	GameObject* enemy = new GameObject(&mPlayerTexture, mCamera, enemyCollider);
	enemy->SetPosition(Vector2(300, 1));

	mGameObjects.push_back(player);
	mGameObjects.push_back(bullet);
	mGameObjects.push_back(enemy);
	int playerWidth = player->GetWidth();
	int playerHeight = player->GetHeight();

	//While application is running
	while (!quit)
	{
		// Get deltaTime
		Uint32 now = SDL_GetTicks();
		Uint32 deltaTime = now - last;
		last = now;

		//Handle events on queue
		while (SDL_PollEvent(&e) != 0)
		{
			//User requests quit
			if (e.type == SDL_QUIT)
			{
				quit = true;
			}
			if (e.type == SDL_MOUSEBUTTONDOWN) {
				int mouseX;
				int mouseY;
				SDL_GetMouseState(&mouseX, &mouseY);
				Vector2 mousePos(mouseX, mouseY);
				Vector2 shootDirection(mousePos - player->GetPosition());
				shootDirection.Normalize();
				bullet->SetActive(true);
				bullet->Shoot(player->GetPosition(), shootDirection);
			}
		}



		Vector2 dir(0, 0);
		player->Move(dir);

		const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
		if (currentKeyStates[SDL_SCANCODE_W])
		{
			dir += Vector2(0, -1);
		}
		else if (currentKeyStates[SDL_SCANCODE_S])
		{
			dir += Vector2(0, 1);
		}
		if (currentKeyStates[SDL_SCANCODE_A])
		{
			dir += Vector2(-1, 0);
		}
		else if (currentKeyStates[SDL_SCANCODE_D])
		{
			dir += Vector2(1, 0);
		}

		player->Move(dir);

		//Center the camera over the dot
		mCamera.x = (player->GetPosition().x + playerWidth / 2) - SCREEN_WIDTH / 2;
		mCamera.y = (player->GetPosition().y + playerHeight / 2) - SCREEN_HEIGHT / 2;

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

		//Clear screen
		SDL_SetRenderDrawColor(mRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
		SDL_RenderClear(mRenderer);

		//player->Update(deltaTime);
		mBackgroundTexture.Render(0, 0, &mCamera);

		for (GameObject* g : mGameObjects)
		{
			g->Update(deltaTime);
		}

		//Update screen
		SDL_RenderPresent(mRenderer);

	}

	//Free resources and close SDL
	Close();
}
