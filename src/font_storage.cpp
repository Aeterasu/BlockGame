#include "font_storage.h"

#include "log.h"

namespace blockgame
{
	FontStorage fontStorage;

	void InitFontStorage()
	{
		fontStorage.m3x6 = TTF_OpenFont("assets/fonts/m3x6.ttf", 16);
		if (!fontStorage.m3x6)
		{
			log_err("Font load failed: ", TTF_GetError());
		}

		fontStorage.m5x7 = TTF_OpenFont("assets/fonts/m5x7.ttf", 16);
		if (!fontStorage.m5x7)
		{
			log_err("Font load failed: ", TTF_GetError());
		}

		fontStorage.m6x11 = TTF_OpenFont("assets/fonts/m6x11.ttf", 22);
		if (!fontStorage.m6x11)
		{
			log_err("Font load failed: ", TTF_GetError());
		}

		fontStorage.silver = TTF_OpenFont("assets/fonts/silver.ttf", 16);
		if (!fontStorage.silver)
		{
			log_err("Font load failed: ", TTF_GetError());
		}

		fontStorage.superstar = TTF_OpenFont("assets/fonts/superstar.ttf", 16);
		if (!fontStorage.superstar)
		{
			log_err("Font load failed: ", TTF_GetError());
		}
	}
} // namespace blockgame
