#pragma once

struct SDLWindowDestroyer
{
	void operator()(SDL_Window* w) const
	{
		SDL_DestroyWindow(w);
	}
};

struct SDLRendererDestroyer
{
	void operator()(SDL_Renderer* r) const
	{
		SDL_DestroyRenderer(r);
	}
};