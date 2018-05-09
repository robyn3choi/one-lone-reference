#include "Precompiled.h"
#include "TextureManager.h"

TextureManager::TextureManager(SDL_Renderer* renderer) :
	mRenderer(renderer)
{
	LoadTextures();
	LoadTTFTextures();
}


TextureManager::~TextureManager()
{
}

Texture * TextureManager::GetTexture(TextureType t)
{
	return mTextures[static_cast<int>(t)];
}

void TextureManager::LoadTextures()
{
	mPaths = {
		"Images/bg.png",
		"Images/reference.png",
		"Images/playerBullet.png",
		"Images/pointer.png",
		"Images/enemyBullet.png",
		"Images/bricks2.png",
		"Images/crosshair.png",
		"Images/GameOverText.png",
		"Images/TryAgainButton.png",
		"Images/heart.png"
	};

	for (size_t i = 0; i < mPaths.size(); i++)
	{
		Texture* t = new Texture(mRenderer);
		t->LoadFromFile(mPaths[i]);
		mTextures.push_back(t);
	}
}

void TextureManager::LoadTTFTextures()
{
	//std::vector<std::string> textArray = {
	//	"Game Over",
	//	"Try Again"
	//};

	//TTF_Font* font = TTF_OpenFont("Fonts/manaspc.ttf", 28);

	//Texture* gameOverText = new Texture(mRenderer);
	//gameOverText->LoadFromRenderedText(font, "Game Over", { 255,255,255 });

}
