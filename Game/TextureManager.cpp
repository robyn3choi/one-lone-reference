#include "Precompiled.h"
#include "TextureManager.h"

TextureManager::TextureManager(SDL_Renderer* renderer) :
	mRenderer(renderer)
{
	LoadTextures();
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
		"Images/heart.png",
		"Images/boss.png",
		"Images/healthBarFill.png",
		"Images/healthBarBackground.png",
		"Images/Text/bossText1.png",
		"Images/Text/bossText2.png",
		"Images/Text/refText1.png",
		"Images/Text/refText2.png",
		"Images/deadBossPointer.png",
		"Images/cryingReference.png"
	};

	for (size_t i = 0; i < mPaths.size(); i++)
	{
		Texture* t = new Texture(mRenderer);
		t->LoadFromFile(mPaths[i]);
		mTextures.push_back(t);
	}
}
