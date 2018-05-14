#pragma once
#include <string>
#include <vector>
#include "Texture.h"

enum class TextureType
{
	Background,
	Player,
	PlayerBullet,
	Enemy,
	EnemyBullet,
	Ground,
	Crosshair,
	GameOverText,
	TryAgainButton,
	Heart,
	Boss,
	BossHealthFill,
	BossHealthBackground,
	BossText1,
	BossText2,
	PlayerText1,
	PlayerText2,
	DeadBoss,
	CryingPlayer

};

enum class TTFTextureType
{
	GameOver,
	TryAgain
};

class TextureManager
{
public:
	TextureManager(SDL_Renderer* renderer);
	~TextureManager();

	Texture* GetTexture(TextureType t);

private:
	void LoadTextures();
	void LoadTTFTextures();

	SDL_Renderer* mRenderer;
	std::vector<std::string> mPaths;
	std::vector<Texture*> mTextures;
	std::vector<Texture*> mTTFTextures;
};