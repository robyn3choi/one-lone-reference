#include "Precompiled.h"
#include "TextureManager.h"

TextureManager::TextureManager(SDL_Renderer*& renderer) :
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
		"Images/testImage.png",
		"Images/cursor.png",
		"Images/testImage.png",
		"Images/cursor.png",
		"Images/bricks2.png",
		"Images/circle.png",
		"Images/GameOverText.png",
		"Images/TryAgainButton.png"
	};

	for (int i = 0; i < mPaths.size(); i++)
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
