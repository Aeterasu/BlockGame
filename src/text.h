#pragma once
#include "quad.h"
#include "texture.h"

#include <SDL2/SDL_ttf.h>
#include <string>

namespace blockgame
{
	struct Label
	{
		TTF_Font* font;
		SDL_Color color;
		Texture texture;
		Quad quad;
		QuadHandle handle = 0;
		std::string lastText;

		void Create(TTF_Font* font, const std::string& text, SDL_Color color, glm::vec2 position,
					std::int32_t zIndex = 0);

		void SetText(const std::string& text);
	};

	Texture CreateTextTexture(TTF_Font* font, const std::string& text, SDL_Color color);

	std::string FormatScore(uint64_t score);
} // namespace blockgame
