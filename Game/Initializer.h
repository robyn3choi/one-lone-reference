#pragma once
#include "Player.h"

class Initializer
{
public:
	Initializer();
	~Initializer();

	bool InitializeSDL();
	bool LoadMedia();
	bool CreateGameObjects();

private:
	SDL_Window* mWindow = nullptr;
	SDL_Renderer* mRenderer = nullptr;
	// textures
	Texture mPlayerTexture;
	Texture mPlayerBulletTexture;
	Texture mBackgroundTexture;
	// colliders
	SDL_Rect mPlayerCollider;
	SDL_Rect mPlayerBulletCollider;
	Player mPlayer;
};

