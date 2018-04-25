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
	Cursor,
	GameOverText,
	TryAgainButton
};

enum class TTFTextureType
{
	GameOver,
	TryAgain
};

class TextureManager
{
public:
	TextureManager(SDL_Renderer*& renderer);
	~TextureManager();

	Texture* GetTexture(TextureType t);

private:
	void LoadTextures();
	void LoadTTFTextures();

	SDL_Renderer*& mRenderer;
	std::vector<std::string> mPaths;
	std::vector<Texture*> mTextures;
	std::vector<Texture*> mTTFTextures;
};