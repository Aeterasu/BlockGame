#include "font_storage.h"

#include <iostream>

namespace blockgame
{
	FontStorage fontStorage;

	void InitFontStorage()
	{
		fontStorage.m3x6 = TTF_OpenFont("assets/fonts/m3x6.ttf", 16);
		if (!fontStorage.m3x6)
		{
			std::cout << "Font load failed: " << TTF_GetError() << "\n";
		}

		fontStorage.m5x7 = TTF_OpenFont("assets/fonts/m5x7.ttf", 16);
		if (!fontStorage.m5x7)
		{
			std::cout << "Font load failed: " << TTF_GetError() << "\n";
		}

		fontStorage.m6x11 = TTF_OpenFont("assets/fonts/m6x11.ttf", 22);
		if (!fontStorage.m6x11)
		{
			std::cout << "Font load failed: " << TTF_GetError() << "\n";
		}

		fontStorage.silver = TTF_OpenFont("assets/fonts/silver.ttf", 16);
		if (!fontStorage.silver)
		{
			std::cout << "Font load failed: " << TTF_GetError() << "\n";
		}

		fontStorage.superstar = TTF_OpenFont("assets/fonts/superstar.ttf", 16);
		if (!fontStorage.superstar)
		{
			std::cout << "Font load failed: " << TTF_GetError() << "\n";
		}
	}
} // namespace blockgame
