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
	EnemyBullet
};

class TextureManager
{
public:
	TextureManager(SDL_Renderer*& renderer);
	~TextureManager();

	static Texture* GetTexture(TextureType t);

private:
	SDL_Renderer*& mRenderer;
	std::vector<std::string> mPaths;
	static std::vector<Texture*> mTextures;
};