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
	void CreateGameObjects();
	void Run();

private:
	SDL_Window* mWindow = nullptr;
	SDL_Renderer* mRenderer = nullptr;
	SDL_Rect mCamera;
	Texture mPlayerTexture;
	Texture mPlayerBulletTexture;
	Texture mBackgroundTexture;
	std::vector<GameObject*> mActiveGameObjects;
};

