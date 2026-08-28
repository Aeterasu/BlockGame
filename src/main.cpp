#include "color.h"
#include "font_storage.h"
#include "game.h"
#include "pico_palette.h"
#include "renderer.h"
#include "shader_storage.h"
#include "text.h"
#include "texture_storage.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL_timer.h>
#include <SDL_video.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

namespace
{
	bool running = true;

	std::uint64_t lastTicks = 0;
	constexpr double targetFps = 60.0f;
	constexpr double fixedDeltaTime = 1.0 / targetFps;

	double fpsUpdateTimer = 0.0;
	std::uint32_t fpsFrameCount = 0;
	std::uint32_t displayedFps = 0;

	blockgame::Label fpsCounterLabel;

	blockgame::Game game;

	bool InitGame()
	{
		game.Init();

		return true;
	}

	void Tick()
	{
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
			{
				running = false;
			}
			else if (event.type == SDL_KEYDOWN)
			{
				if (event.key.repeat == 0)
				{
					if (event.key.keysym.sym == SDLK_ESCAPE)
					{
						running = false;
					}
					else
					{
						switch (event.key.keysym.sym)
						{
							case SDLK_UP:
								game.MoveCursor(glm::ivec2(0, -1));
								break;
							case SDLK_DOWN:
								game.MoveCursor(glm::ivec2(0, 1));
								break;
							case SDLK_LEFT:
								game.MoveCursor(glm::ivec2(-1, 0));
								break;
							case SDLK_RIGHT:
								game.MoveCursor(glm::ivec2(1, 0));
								break;
							case SDLK_z:
								game.isDragging = true;
								break;
							case SDLK_x:
								game.PlaceBomb();
								break;
						}
					}
				}
			}
			else if (event.type == SDL_KEYUP)
			{
				if (event.key.keysym.sym == SDLK_z)
				{
					game.isDragging = false;
				}
			}
			else if (event.type == SDL_WINDOWEVENT)
			{
				if (event.window.event == SDL_WINDOWEVENT_RESIZED || event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
				{
					blockgame::renderer.OnResize(event.window.data1, event.window.data2);
				}
			}
		}

		std::uint64_t now = SDL_GetPerformanceCounter();
		double deltaTime = (double)(now - lastTicks) / (double)SDL_GetPerformanceFrequency();
		lastTicks = now;

		fpsUpdateTimer += deltaTime;
		fpsFrameCount++;

		if (fpsUpdateTimer >= 1.0)
		{
			displayedFps = static_cast<uint32_t>(fpsFrameCount / fpsUpdateTimer);
			fpsUpdateTimer = 0.0;
			fpsFrameCount = 0;
			fpsCounterLabel.SetText("FPS: " + std::to_string(displayedFps));
		}

		static double accumulator = 0.0;
		accumulator += deltaTime;

		while (accumulator >= fixedDeltaTime)
		{
			game.Tick(fixedDeltaTime);
			accumulator -= fixedDeltaTime;
		}

		blockgame::renderer.DrawFrame();

		SDL_GL_SwapWindow(blockgame::renderer.window);

#ifdef __EMSCRIPTEN__
		if (!running)
		{
			emscripten_cancel_main_loop();
		}
#endif
	}
} // namespace

void CreateGlobalVisuals()
{
	// letterbox bg

	blockgame::renderer.SetLetterboxBackground(&blockgame::textureStorage.letterboxBg);

	// border

	blockgame::Sprite border;
	border.size = glm::vec2(270.0f, 270.0f);
	border.texture = &blockgame::textureStorage.border;
	border.shader = &blockgame::shaderStorage.spriteShader;
	border.zIndex = 999999;
	blockgame::Color grey = blockgame::PICO_GREY;
	border.tint = glm::vec4(grey.r, grey.g, grey.b, 1.0);
	blockgame::renderer.AddSprite(border);

	// fps counter

	fpsCounterLabel.Create(blockgame::fontStorage.m3x6, "FPS: 0", blockgame::ColorToSDLColor(blockgame::PICO_GREY),
						   &blockgame::shaderStorage.spriteShader, glm::vec2(4.0f, 0.0f), border.zIndex + 1);
}

int main()
{
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		std::cout << "SDL_Init failed: " << SDL_GetError() << "\n";
	}

	if (TTF_Init() != 0)
	{
		std::cout << "TTF_Init failed: " << TTF_GetError() << "\n";
	}

	bool gl = blockgame::renderer.InitGL();

	if (!gl)
	{
		SDL_Quit();
		return -1;
	}

	lastTicks = SDL_GetPerformanceCounter();

	blockgame::InitTextureStorage();
	blockgame::InitShaderStorage();
	blockgame::InitFontStorage();

	CreateGlobalVisuals(); // border and other screen space stuff we may have

	InitGame();

#ifdef __EMSCRIPTEN__
	emscripten_set_main_loop(Tick, 0, 1);
#else
	while (running)
	{
		Tick();
	}
#endif
	blockgame::renderer.DeleteGL();
	SDL_Quit();
	return 0;
}
