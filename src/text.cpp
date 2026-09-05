#include "text.h"

#include "renderer.h"

#include <iostream>

namespace blockgame
{
	Texture CreateTextTexture(TTF_Font* font, const std::string& text, SDL_Color color)
	{
		if (!font || text.empty())
		{
			return Texture{};
		}

		SDL_Surface* surface = TTF_RenderUTF8_Blended(font, text.c_str(), color);

		if (surface == nullptr)
		{
			std::cout << "TTF_RenderUTF8_Blended failed: " << TTF_GetError() << "\n";
			return Texture{};
		}

		SDL_Surface* converted = SDL_ConvertSurfaceFormat(surface, SDL_PIXELFORMAT_RGBA32, 0);
		SDL_FreeSurface(surface);

		if (converted == nullptr)
		{
			std::cout << "SDL_ConvertSurfaceFormat failed: " << SDL_GetError() << "\n";
			return Texture{};
		}

		GLuint id = 0;
		glGenTextures(1, &id);
		glBindTexture(GL_TEXTURE_2D, id);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, converted->w, converted->h, 0, GL_RGBA, GL_UNSIGNED_BYTE,
					 converted->pixels);
		glBindTexture(GL_TEXTURE_2D, 0);

		Texture texture;
		texture.id = id;
		texture.width = converted->w;
		texture.height = converted->h;

		SDL_FreeSurface(converted);
		return texture;
	}

	void Label::Create(TTF_Font* font_, const std::string& text_, SDL_Color color_,
					   glm::vec2 position_, std::int32_t zIndex_)
	{
		texture = CreateTextTexture(font_, text_, color_);
		lastText = text_;

		font = font_;
		color = color_;

		quad.position = position_;
		quad.size = glm::vec2((float)texture.width, (float)texture.height);
		quad.ApplyTexture(&texture);
		quad.zIndex = zIndex_;

		handle = renderer.AddQuad(quad);
	}

	void Label::SetText(const std::string& text)
	{
		if (text == lastText)
		{
			return;
		}

		if (texture.id != 0)
		{
			glDeleteTextures(1, &texture.id);
		}

		texture = CreateTextTexture(font, text, color);
		lastText = text;

		quad.size = glm::vec2((float)texture.width, (float)texture.height);
		renderer.UpdateQuad(handle, quad);
	}

	std::string FormatScore(uint64_t score)
	{
		std::string s = std::to_string(score);

		size_t len = s.length();

		if (len <= 3)
		{
			return s;
		}

		size_t insert_position = len - 3;

		while (insert_position > 0)
		{
			s.insert(insert_position, ",");

			if (insert_position < 3)
			{
				break;
			}

			insert_position -= 3;
		}

		return s;
	}
} // namespace blockgame
