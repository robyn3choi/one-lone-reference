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
	Cursor
};

class TextureManager
{
public:
	TextureManager(SDL_Renderer*& renderer);
	~TextureManager();

	Texture* GetTexture(TextureType t);

private:
	SDL_Renderer*& mRenderer;
	std::vector<std::string> mPaths;
	std::vector<Texture*> mTextures;
};