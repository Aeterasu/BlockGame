#include "game.h"

#include <SDL2/SDL.h>
#include <SDL_image.h>
#include <SDL_render.h>
#include <stdio.h>

int main()
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        fprintf(stderr, "SDL_Init failed: %s\n", SDL_GetError());
    }

    IMG_Init(IMG_INIT_PNG);

    SDL_Window* window =
        SDL_CreateWindow("BlockGame", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 270, 270, SDL_WINDOW_SHOWN);

    if (window == nullptr)

    {
        fprintf(stderr, "Game window could not be created, aborting...");
        return -1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    bool running = true;
    SDL_Event event;

    BlockGame::Game game;
    game.init();

    SDL_Surface* surface = IMG_Load("assets/textures/texture_grid.png");

    if (!surface)
    {
        fprintf(stderr, "IMG_Load failed: %s\n", IMG_GetError());
        return -1;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    while (running)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                running = false;
            }
        }

        SDL_SetRenderDrawColor(renderer, 95, 87, 79, 255);
        SDL_RenderClear(renderer);

        SDL_Rect destRect{0, 0, 270, 270};
        SDL_RenderCopy(renderer, texture, nullptr, &destRect);

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyWindow(window);
    SDL_Quit();

    IMG_Quit();

    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);

    return 0;
}
