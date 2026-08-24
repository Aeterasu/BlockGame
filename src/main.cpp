#include "game.h"
#include "renderer.h"
#include "texture_storage.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL_video.h>
#include <cstdio>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace
{
    bool running = true;

    blockgame::Game game;

    bool InitGame()
    {
        game.init();

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

    blockgame::InitTextureStorage();

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
