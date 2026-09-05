#pragma once

#include "quad.h"

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

		Quad letterboxBackground;
		bool hasLetterboxBackground = false;

		std::vector<Quad> activeQuads;
		std::vector<QuadHandle> handles;

		QuadHandle nextHandle = 0;

		GLuint quadVao = 0;
		glm::mat4 projection{1.0f};
		glm::mat4 windowProjection;

		QuadHandle AddQuad(const Quad& quad);
		void RemoveQuad(QuadHandle handle);
		void UpdateQuad(QuadHandle handle, const Quad& quad);

		void InitQuad();

		bool InitGL();

		void DrawFrame();

		void DeleteGL();

		void OnResize(int width, int height);

		void SetLetterboxBackground(Texture* texture);
	};

	extern Renderer renderer;
} // namespace blockgame
