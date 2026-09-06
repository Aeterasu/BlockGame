#include "texture.h"

#include "log.h"

#include <SDL_image.h>

namespace blockgame
{
	Texture LoadTexture(const std::string& filename)
	{
		std::string path = "assets/textures/" + filename;
		SDL_Surface* surface = IMG_Load(path.c_str());

		if (surface == nullptr)
		{
			log_err("IMG_Load failed for ", path, ": ", IMG_GetError());
			return Texture{};
		}

		SDL_Surface* converted = SDL_ConvertSurfaceFormat(surface, SDL_PIXELFORMAT_RGBA32, 0);
		SDL_FreeSurface(surface);

		if (converted == nullptr)
		{
			log_err("SDL_ConvertSurfaceFormat failed for ", path, ": ", SDL_GetError());

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
} // namespace blockgame
