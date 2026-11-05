#include <stdio.h>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

#include <SDL3/SDL_main.h>

#include "video.h"

bool is_running = true;

static void teardown(void)
{
	video_deinit();
}

static void tick(void)
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_EVENT_QUIT)
		{
			is_running = false;
#ifdef __EMSCRIPTEN__
			emscripten_cancel_main_loop();
#endif
		}
	}

	video_clear_frame();
	video_render_frame();
}

int main(int argc, char* argv[])
{
	video_init();
	video_upload_texture_atlas("asset/texture/texture_1.png");

#ifdef __EMSCRIPTEN__
	emscripten_set_main_loop(tick, 0, 1);
#else
	while (is_running)
	{
		tick();
	}

	teardown();
#endif

	return 0;
}
