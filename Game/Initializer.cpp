#include "Precompiled.h"
#include "Initializer.h"


Initializer::Initializer() :
	mPlayerTexture(mRenderer),
	mPlayerBulletTexture(mRenderer),
	mBackgroundTexture(mRenderer),
	mPlayer(mPlayerTexture, mPlayerCollider, PLAYER_SPEED)
{
}


Initializer::~Initializer()
{
}

bool Initializer::InitializeSDL()
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

bool Initializer::LoadMedia()
{
	bool success = true;

	if (!mPlayerTexture.LoadFromFile("Images/testImage.png"))
	{
		printf("Failed to load player texture!\n");
		success = false;
	}

	if (!mPlayerBulletTexture.LoadFromFile("Images/cursor.png"))
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

bool Initializer::CreateGameObjects()
{
	
}
