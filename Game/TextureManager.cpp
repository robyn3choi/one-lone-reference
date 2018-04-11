#include "Precompiled.h"
#include "TextureManager.h"

TextureManager::TextureManager(SDL_Renderer*& renderer) :
	mRenderer(renderer)
{
	std::string pathsArray[] = {
		"Images/bg.png",
		"Images/testImage.png",
		"Images/cursor.png",
		"Images/testImage.png",
		"Images/cursor.png",
		"Images/bricks2.png"
	};

	mPaths = std::vector<std::string>(std::begin(pathsArray), std::end(pathsArray));

	for (int i = 0; i < mPaths.size(); i++)
	{
		Texture* t = new Texture(renderer);
		t->LoadFromFile(mPaths[i]);
		mTextures.push_back(t);
	}
}


TextureManager::~TextureManager()
{
}

Texture * TextureManager::GetTexture(TextureType t)
{
	return mTextures[static_cast<int>(t)];
}
