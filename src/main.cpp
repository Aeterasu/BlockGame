#include "color.h"
#include "game.h"
#include "pico_palette.h"
#include "renderer.h"
#include "shader_storage.h"
#include "texture_storage.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL_timer.h>
#include <SDL_video.h>
#include <cstdio>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace
{
    bool running = true;

    std::uint64_t lastTicks = 0;
    constexpr double targetFps = 60.0f;
    constexpr double fixedDeltaTime = 1.0 / targetFps;

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
                        case SDLK_w:
                            game.MoveCursor(glm::ivec2(0, -1));
                            break;
                        case SDLK_s:
                            game.MoveCursor(glm::ivec2(0, 1));
                            break;
                        case SDLK_a:
                            game.MoveCursor(glm::ivec2(-1, 0));
                            break;
                        case SDLK_d:
                            game.MoveCursor(glm::ivec2(1, 0));
                            break;
                        }
                    }
                }
            }
        }

        std::uint64_t now = SDL_GetPerformanceCounter();
        double deltaTime = (double)(now - lastTicks) / (double)SDL_GetPerformanceFrequency();
        lastTicks = now;

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
    blockgame::Sprite border;
    border.size = glm::vec2(270.0f, 270.0f);
    border.texture = &blockgame::textureStorage.border;
    border.shader = &blockgame::shaderStorage.spriteShader;
    border.zIndex = 999999;
    blockgame::Color grey = blockgame::PICO_GREY;
    border.tint = glm::vec4(grey.r, grey.g, grey.b, 1.0);
    blockgame::renderer.AddSprite(border);
}

int main()
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        std::fprintf(stderr, "SDL_Init failed: %s\n", SDL_GetError());
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
