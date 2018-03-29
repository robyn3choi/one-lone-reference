#pragma once
#include <vector>
#include "GameObject.h"

class GameManager
{
public:
	GameManager();
	~GameManager();

	bool Initialize();
	bool LoadMedia();
	void Close();
	void Run();

private:
	//Screen dimension constants
	const int SCREEN_WIDTH = 1280;
	const int SCREEN_HEIGHT = 720;
	//The dimensions of the level
	const int LEVEL_WIDTH = 3000;
	const int LEVEL_HEIGHT = 2000;

	const int PLAYER_BULLET_POOL_SIZE = 16;
	const int ENEMY_BULLET_POOL_SIZE = 36;

	SDL_Window* mWindow = nullptr;
	SDL_Renderer* mRenderer = nullptr;
	SDL_Rect mCamera;
	Texture mPlayerTexture;
	Texture mBulletTexture;
	Texture mBackgroundTexture;
	std::vector<GameObject*> mActiveGameObjects;
};

