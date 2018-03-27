// adapted from lazyfoo.net

#pragma once

class Texture
{
public:
	Texture(SDL_Renderer*& renderer);
	~Texture();

	//Loads image at specified path
	bool LoadFromFile(std::string path);

	//Deallocates texture
	void Free();

	//Set color modulation
	void SetColor(Uint8 red, Uint8 green, Uint8 blue);

	//Set blending
	void SetBlendMode(SDL_BlendMode blending);

	//Set alpha modulation
	void SetAlpha(Uint8 alpha);

	//Renders texture at given point
	void Render(int x, int y, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);

	//Gets image dimensions
	int GetWidth();
	int GetHeight();

private:
	SDL_Renderer*& mRenderer;
	//The actual hardware texture
	SDL_Texture* mTexture = NULL;

	//Image dimensions
	int mWidth = 0;
	int mHeight = 0;
};

