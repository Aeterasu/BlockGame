#pragma once

#include <SDL2/SDL_ttf.h>

namespace blockgame
{
	struct FontStorage
	{
		TTF_Font* m3x6;
		TTF_Font* m5x7;
		TTF_Font* m6x11;
		TTF_Font* silver;
		TTF_Font* superstar;
	};

	extern FontStorage fontStorage;

	void InitFontStorage();
} // namespace blockgame
