#include <SDL2/SDL.h>
#include <stdio.h>

int main()
{
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		fprintf(stderr, "SDL_Init failed: %s\n", SDL_GetError());
	}

	SDL_Window* window = SDL_CreateWindow(
		"BlockGame",
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		256, 256,
		SDL_WINDOW_SHOWN
	);
	
	if (window == nullptr)
	{
		fprintf(stderr, "Game window could not be created, aborting...");
		return -1;
	}

	bool running = true;
	SDL_Event event;
	while (running)
	{
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
			{
				running = false;
			}
		}
	}

	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}
