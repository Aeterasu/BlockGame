#pragma once

#include "sprite.h"

#include <SDL2/SDL.h>
#include <glm/glm.hpp>
#include <vector>

namespace blockgame
{
	struct Renderer
	{
		SDL_Window* window = nullptr;
		SDL_GLContext glContext = nullptr;

		int windowWidth = 270;
		int windowHeight = 270;

		SDL_Rect viewport = {0, 0, 270, 270};

		const bool ENABLE_VSYNC = false;

		Sprite letterboxBackground;
		bool hasLetterboxBackground = false;

		std::vector<Sprite> activeSprites;
		std::vector<SpriteHandle> handles;

		SpriteHandle nextHandle = 0;

		GLuint quadVao = 0;
		glm::mat4 projection{1.0f};
		glm::mat4 windowProjection;

		SpriteHandle AddSprite(const Sprite& sprite);
		void RemoveSprite(SpriteHandle handle);
		void UpdateSprite(SpriteHandle handle, const Sprite& sprite);

		void InitQuad();

		bool InitGL();

		void DrawFrame();

		void DeleteGL();

		void OnResize(int width, int height);

		void SetLetterboxBackground(Texture* texture);
	};

	extern Renderer renderer;
} // namespace blockgame
