#pragma once
#include "shader.h"
#include "sprite.h"
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
		Sprite sprite;
		SpriteHandle handle = 0;
		std::string lastText;

		void Create(TTF_Font* font, const std::string& text, SDL_Color color, const Shader* shader, glm::vec2 position,
					std::int32_t zIndex = 0);

		void SetText(const std::string& text);
	};

	Texture CreateTextTexture(TTF_Font* font, const std::string& text, SDL_Color color);
} // namespace blockgame
